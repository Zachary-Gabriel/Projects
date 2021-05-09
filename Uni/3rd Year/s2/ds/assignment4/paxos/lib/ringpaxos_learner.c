#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "lp_learner.h"
#include "lp_network.h"
#include "lp_config_parser.h"
#include "lp_topology.h"
#include "lp_utils.h"
#include "ringpaxos_messages.h"

struct learner_event_counters {
	long unsigned map_request;
	long unsigned chosenval_request;
};

static bool initialized = false;
static struct learner_event_counters lec;
static udp_sender * mcast_send = NULL;
static udp_receiver * mcast_recv = NULL;
static delivery_queue * dq = NULL;

void learner_print_eventcounters() {
	PRINT_COUNT(lec.map_request);
	PRINT_COUNT(lec.chosenval_request);
	printf("To multicaster:\n");
	udp_sender_print_stats(mcast_send, time(NULL));
	printf("From multicaster\n");
	udp_receiver_print_stats(mcast_recv, time(NULL));
}

void on_configuration_change(config_mngr * old, config_mngr * new) {
    LOG_MSG(INFO, ("Configuration changed!\n"));
    // TODO low_priority
    UNUSED_ARG(old);
    UNUSED_ARG(new);
}

void on_topology_change() {
    LOG_MSG(INFO, ("Topology changed!\n"));
    
    // TODO low_priority
	//Re-connect to some acceptor for retransmissions
}

static char missing_cmd_request_buf[MAX_MESSAGE_SIZE];
void on_missing_cmdmap(iid_t inst_number) {
	map_requests_msg * msg = (map_requests_msg*)&missing_cmd_request_buf;
	COUNT_EVENT(PAXOS, lec.map_request);
	msg->inst_number[msg->requests_count] = inst_number;
	msg->requests_count += 1;
	
	if(msg->requests_count >= REPEAT_REQUEST_MAX_ENTRIES) {
		net_send_udp(mcast_send, msg, CMDMAP_REQS_MSG_SIZE(msg), map_request);		
		udp_sender_force_flush(mcast_send);
		msg->requests_count = 0;
	}	
}

static char missing_acc_request_buf[MAX_MESSAGE_SIZE];
void on_missing_acceptance(iid_t inst_number) {
	chosencmd_requests_msg * msg = (chosencmd_requests_msg*)&missing_acc_request_buf;
	COUNT_EVENT(PAXOS, lec.chosenval_request);
	msg->inst_number[msg->requests_count] = inst_number;
	msg->requests_count += 1;
	
	if(msg->requests_count >= REPEAT_REQUEST_MAX_ENTRIES) {
		net_send_udp(mcast_send, msg, FINVAL_REQS_MSG_SIZE(msg), chosenval_request);		
		udp_sender_force_flush(mcast_send);
		msg->requests_count = 0;		
	}
}

//Invoked after the missing_map and missing_acceptance may have been
// called to fill gaps, flush send buffer
void post_delivery_check() {
	map_requests_msg * msg1 = (map_requests_msg*)&missing_cmd_request_buf;
	chosencmd_requests_msg * msg2 = (chosencmd_requests_msg*)&missing_acc_request_buf;

	if(msg1->requests_count > 0) {
		net_send_udp(mcast_send, msg1, CMDMAP_REQS_MSG_SIZE(msg1), map_request);		
		msg1->requests_count = 0;
	}
	if(msg2->requests_count > 0) {
		net_send_udp(mcast_send, msg2, FINVAL_REQS_MSG_SIZE(msg2), chosenval_request);
		msg2->requests_count = 0;
	}
	
	udp_sender_force_flush(mcast_send);
}


void on_mcast_msg(void* data, size_t size, lp_msg_type type) {
    
    switch(type) {
        case command_map: {
            cmdmap_msg* msg = data;
            assert(size == CMDMAP_MSG_SIZE(msg));
            delivery_queue_handle_command_map(dq, 
                msg->inst_number, &msg->cmd_key, 
                msg->cmd_size, msg->cmd_value);         
        }
        break;
            
        case acceptance: {
            acceptance_msg* msg = data;
            assert(size == sizeof(acceptance_msg));
            delivery_queue_handle_acceptance(dq, msg->inst_number, &msg->cmd_key);
        }
        break;
        
        default: 
        LOG_MSG(DEBUG, ("Dropping message of type %d\n", type));
    }
}

// Invoked after a multicast message has been processed
void post_receive_check() {
	dq_deliver_loop(dq);
}

int learner_init(
	const char * config_file_path, 
	custom_init_func cust_init, 
	deliver_callback dcb) 
{

	assert(initialized == false);
	initialized = true;
	
    // Parse configuration file
    int result = config_mngr_init(
        config_file_path, /*Path to config file*/
        0,           /*Numeric ID of this acceptor, -1 since it's a learner*/
        on_configuration_change /*Callback for config update*/
        );
    assert(result == 0);
    LOG_MSG(DEBUG, ("Configuration manager initialized!\n"));
    
    // Set event for topology change
    result = topology_mngr_init(
        on_topology_change   /*Callback for ring update*/
    );
	assert(result == 0);
    LOG_MSG(DEBUG, ("Topology manager initialized!\n"));
    
    // Create delivery queue
    dq = delivery_queue_init( 
		dcb, 
		on_missing_cmdmap, 
		on_missing_acceptance, 
		post_delivery_check);
	assert(dq != NULL);
	LOG_MSG(DEBUG, ("Delivery queue initialized!\n"));
    
    // Connect to some multicaster to request retransmissions
    //TODO instead of asking everything to the multicaster, should:
    // - ask missed mappings to multicaster
    // - ask final values to some acceptor with TCP
    mcast_send = udp_sender_init(
        lptopo_get_leader_addr(),       /*Remote address string*/
        lptopo_get_leader_ring_port()             /*Remote port*/
        );
	assert(mcast_send != NULL);
	//No autoflush, flush triggered in post_delivery_check
	LOG_MSG(DEBUG, ("Socket to Multicaster initialized\n"));
        
    // Open listen port for multicast
    // Set event for multicast messages
    mcast_recv = mcast_receiver_init(
        lpconfig_get_mcast_addr(),       /*Listen address*/
        lpconfig_get_mcast_port(),             /*Listen port*/
        on_mcast_msg /*Callback for packet received*/
        ); 
    assert(mcast_recv != NULL);
	udp_receiver_set_postdeliver_callback(mcast_recv, post_receive_check);
	LOG_MSG(DEBUG, ("Multicas receiver initialized\n"));
	
	//Clear event counters structure
	memset(&lec, '\0', sizeof(struct learner_event_counters));
	
	if(cust_init != NULL) {
		LOG_MSG(DEBUG, ("Invoking custom initialization of base_learner\n"));
		cust_init();
	}

	return 0;
	
}

