#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "paxos_config.h"
#include "lp_config_parser.h"
#include "lp_topology.h"
#include "lp_network.h"
#include "lp_utils.h"
#include "lp_submit_proxy.h"

#define LARGEST_PAXOS_HEADER 14 //Phase1 

struct submit_proxy_t {
	udp_sender * us;
	char buf[MAX_MESSAGE_SIZE];
};

static void on_topology_change_proxy() {
	// TODO low priority
};

static void on_configuration_change_proxy() {
	// TODO low priority
};

submit_proxy * submit_proxy_udp_init(const char * config_path) {
		
	submit_proxy * sp = calloc(1, sizeof(submit_proxy));
	assert(sp != NULL);
	
	int result;
	
    // Parse configuration file
    result = config_mngr_init(
        config_path, /*Path to config file*/
        0,           /*Numeric ID of this acceptor*/
        on_configuration_change_proxy /*Callback for config update*/
        );
    assert(result == 0);
    LOG_MSG(DEBUG, ("Configuration manager initialized!\n"));

    result = topology_mngr_init(
        on_topology_change_proxy   /*Callback for ring update*/
    );
	assert(result == 0);
	
	sp->us = udp_sender_init(
		lptopo_get_leader_addr(), 
		lptopo_get_leader_clients_port());
	assert(sp->us != NULL);	

	return sp;
}

void submit_command(submit_proxy * sp, submit_cmd_msg * scm) {
	if(SUBMIT_CMD_MSG_SIZE(scm) > (MAX_MESSAGE_SIZE - LARGEST_PAXOS_HEADER)) {
		printf("Submit message too big (%lu), dropped!\n", SUBMIT_CMD_MSG_SIZE(scm));
		return;
	}
	
	net_send_udp(sp->us, scm, SUBMIT_CMD_MSG_SIZE(scm), client_submit);
}

void submit_proxy_flush_now(submit_proxy * sp) {
	udp_sender_force_flush(sp->us);
}
