#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
 
#include <event.h>

#include "paxos_config.h"
#include "lp_utils.h"
#include "lp_topology.h"
#include "lp_timers.h"
#include "lp_clients_handling.h"
#include "lp_config_parser.h"
#include "lp_network.h"
#include "lp_delivery_repeat.h"
#include "lp_stable_storage.h"
#include "lp_mcaster_storage.h"
#include "lp_submit_proxy.h"

#include "ringpaxos_messages.h"

//All acceptors
static udp_receiver * pred_recv = NULL;
static udp_sender * succ_send = NULL;
static periodic_event * print_counters_ev = NULL;
static struct timeval print_counters_interval = LP_COUNTERS_PRINT_INTERVAL;

//Only multicaster
static udp_sender * mcast_send = NULL;
static periodic_event * periodic_ev = NULL;
static mcaster_storage_mngr * msm = NULL;

// Only non-multicaster
static stable_storage_mngr * ssm = NULL;
static udp_receiver * mcast_recv = NULL;
static learner_mngr * lm = NULL;


bool am_i_leader() {
    //TODO low_priority
    return (lpconfig_get_self_acceptor_id() == 1);
}

bool am_i_first_in_ring() {
    //TODO low_priority
    return (lpconfig_get_self_acceptor_id() == 2);	
}

//Acceptor events are defined in a separate file
#include "acceptor_helpers.c"
#include "acceptor_helpers_mcaster.c"
#include "acceptor_events.c"

void common_init() {
	
	int result = 0;
		
	// "clean" exit when a SIGINT (ctrl-c) is received
	enable_ctrl_c_handler();
    
	//Initialize libevent
    event_init();
    
    // Set event for topology change
    result = topology_mngr_init(
        on_topology_change   /*Callback for ring update*/
    );
	assert(result == 0);
    LOG_MSG(DEBUG, ("Topology manager initialized!\n"));
    
    //Open UDP connection to successor
    succ_send = udp_sender_init(
        lptopo_get_successor_addr(),  /*Ring addr of successor*/
        lptopo_get_successor_port()   /*Ring port for successor*/
        );
    assert(succ_send != NULL);
	udp_sender_enable_autoflush(succ_send, 1);
    LOG_MSG(DEBUG, ("Successor sender intialized (%s:%d)!\n", 
        lptopo_get_successor_addr(), 
        lptopo_get_successor_port()));


    // Open listen port for predecessor
    // Set event for predecessor messages
    pred_recv = udp_receiver_init(
        lpconfig_get_ring_inbound_addr(),        /*Ring addr address*/
        lpconfig_get_ring_inbound_port(), /*Ring port for predecessor*/
        on_predecessor_msg /*Message from prede is received*/
        );
    assert(pred_recv != NULL);
    

    LOG_MSG(DEBUG, ("Predecessor receiver intialized (%s:%d)!\n", 
        lpconfig_get_ring_inbound_addr(), 
        lpconfig_get_ring_inbound_port()));
}

void multicaster_init() {
	bool success;
	
    LOG_MSG(PAXOS, ("This acceptor (id:%d) is the current leader\n", 
        lpconfig_get_self_acceptor_id()));

    // Open listen socket for clients
    // Set event for client values submission
    //Create an in-memory structure that stores paxos commands (a.k.a. client values)
    success = client_values_mngr_UDP_init(
        mcaster_handle_value_submit /*Called when a client sends a value*/
        );
    assert(success);
	success = client_values_mngr_init_queue(250); //TODO make config parameter
    LOG_MSG(DEBUG, ("Client values manager initialized!\n"));
    
    msm =  mcaster_storage_init();
    assert(msm != NULL);
    LOG_MSG(DEBUG, ("Multicast sender initialized"))

    //Create UDP multicast socket manager
    mcast_send = mcast_sender_init(
        lpconfig_get_mcast_addr(), /*Mcast addr */
        lpconfig_get_mcast_port()  /*Mcast port*/
        );
	//No autoflush, flush happens in mcaster_flush_send_buffers
    assert(mcast_send != NULL);
    LOG_MSG(DEBUG, ("Multicast sender initialized (%s:%d)!\n", 
        lpconfig_get_mcast_addr(), 
        lpconfig_get_mcast_port()));

    // Set periodic event for various routine checks
    periodic_ev = set_periodic_event(
        lpconfig_get_mcaster_clock_interval(),  /*Interval for this event*/
        on_mcaster_periodic_check, /*Called periodically to execute P2*/
        NULL /*Argument passed to above function*/
        );
    assert(periodic_ev != NULL);

	//Update clock when receiving predecessor/learner messages                     
	udp_receiver_set_preread_callback(pred_recv, mcaster_update_wallclock);

	//Init event counters
	if(LP_EVENTCOUNTERS != LOG_NONE) {
		memset(&mec, '\0', sizeof(struct mcaster_event_counters));
		print_counters_ev = set_periodic_event(
			&print_counters_interval,  /*Interval for this event*/
			mcaster_print_counters, /*Called periodically to execute P2*/
			NULL /*Argument passed to above function*/
			);
		assert(print_counters_ev != NULL);
	}

}

void regular_acceptor_init() {
    LOG_MSG(PAXOS, ("This acceptor (id:%d) is NOT the current leader\n",
    lpconfig_get_self_acceptor_id()));
    
    // Init storage: a structure indexed by instance ID
    // that abstract an infinite array
    ssm = stable_storage_init();
    assert(ssm != NULL);
    LOG_MSG(DEBUG, ("Stable storage manager initialized!\n"));

    // Open multicast listen socket, 
    // Set event for multicast messages
    mcast_recv = mcast_receiver_init(
        lpconfig_get_mcast_addr(),              /*Multicast address*/
        lpconfig_get_mcast_port(),                 /*Multicast port*/
        on_multicast_msg /*Called when a message is received*/
        );
    assert(mcast_recv != NULL);
	//Set callback invoked after processing a batch of messages
	//udp_receiver_set_postdeliver_callback(mcast_recv, acceptor_flush_successor_socket);
    LOG_MSG(DEBUG, ("Multicast receiver intialized (%s:%d)!\n", 
        lpconfig_get_mcast_addr(), 
        lpconfig_get_mcast_port()));


	// TODO useless at the moment, remove!
    // Open listen port for learners
    // Set event for learners requests
    lm = learner_mngr_TCP_init(
        lpconfig_get_learners_inbound_addr(), /*Addr for accepting learners connections*/
        lpconfig_get_learners_inbound_port(), /*Port for accepting learners connections*/
        on_repeat_request /*Called when a learner requires a repeat*/
        );
    assert(lm != NULL);
    LOG_MSG(DEBUG, ("Learners manager initialized (%s:%d)!\n", 
        lpconfig_get_learners_inbound_addr(), 
        lpconfig_get_learners_inbound_port()));

	//Set callback invoked after processing a batch of messages
	//udp_receiver_set_postdeliver_callback(pred_recv, acceptor_flush_successor_socket);

	//Init event counters
	if(LP_EVENTCOUNTERS != LOG_NONE) {
		memset(&aec, '\0', sizeof(struct acceptor_event_counters));
		print_counters_ev = set_periodic_event(
			&print_counters_interval,  /*Interval for this event*/
			acceptor_print_counters, /*Called periodically to execute P2*/
			NULL /*Argument passed to above function*/
			);
		assert(print_counters_ev != NULL);
	}
	
}

int main (int argc, char const *argv[]) {
    
    int result;
    const char * config_file_path;
    
    print_cmdline_args(argc, argv);
    
    validate_acceptor_args_or_quit(argc, argv);
    acceptor_id_t acc_id = atoi(argv[1]);
    config_file_path = argv[2];

    // Parse configuration file
    result = config_mngr_init(
        config_file_path, /*Path to config file*/
        acc_id,           /*Numeric ID of this acceptor*/
        on_configuration_change /*Callback for config update*/
        );
    assert(result == 0);
    LOG_MSG(DEBUG, ("Configuration manager initialized!\n"));    

    //Initialization in common for all acceptors
    common_init();
    
    if(am_i_leader()) {
        
        //This acceptor is coordinator/multicaster
        multicaster_init();
        
    } else {
        
        //This acceptor NOT is coordinator/multicaster
        regular_acceptor_init();
    }

    LOG_MSG(INFO, ("Acceptor %d: initialization completed!\n", acc_id));

    //Enter libevent infinite loop
    event_dispatch();
    
    return 0;
}
