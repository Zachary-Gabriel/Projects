#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "forced_assert.h"

#include "lp_utils.h"
#include "lp_config_parser.h"
#include "test_header.h"

int main (int argc, char const *argv[]) {

    UNUSED_ARG(argc);
    UNUSED_ARG(argv);
    
    int result = -1;
    acceptor_id_t acc_id;
    
    // Valid config
    acc_id = 2;
    result = config_mngr_init("./etc/config3.cfg", acc_id, NULL);

    assert(result == 0);

    assert(lpconfig_get_p1_interval()->tv_sec == 1);
    assert(lpconfig_get_p1_interval()->tv_usec == 100);
    assert(lpconfig_get_p2_interval()->tv_sec == 2);
    assert(lpconfig_get_p2_interval()->tv_usec == 200);

    assert(lpconfig_get_acceptors_count() == 3);
    
    assert(strcmp("11.22.33.44", lpconfig_get_ring_inbound_addr()) == 0);
    assert(lpconfig_get_ring_inbound_port() == 7772);
    assert(lpconfig_get_learners_inbound_port() == 5552);
    
    assert(strcmp("111.222.112.221", lpconfig_get_ip_addr_of(3)) == 0);
    assert(lpconfig_get_ring_port_of(3) == 7773);
    assert(lpconfig_get_learners_port_of(3) == 5553);

	assert(strcmp("239.00.0.1", lpconfig_get_mcast_addr()) == 0);
	assert(lpconfig_get_mcast_port() == 6667); 
	
	assert(lpconfig_get_quorum_size() == 2);
	
	//Test that the following have the right default value
	assert(lpconfig_get_delivery_check_interval()->tv_sec == 1); 
	assert(lpconfig_get_delivery_check_interval()->tv_usec == 0); 
	
	assert(lpconfig_get_default_autoflush_interval() == 50); 
	assert(lpconfig_get_working_set_size() == 100); 
	assert(lpconfig_get_max_active_instances() == 25); 
	assert(lpconfig_get_preexecution_window_size() == 50); 
	assert(lpconfig_get_mcaster_clock_interval()->tv_sec == 0); 
	assert(lpconfig_get_mcaster_clock_interval()->tv_usec == 100000);
	
	
	assert(lpconfig_get_max_p2_open_per_iteration() == 10); 
	assert(lpconfig_get_max_client_values_queue_size() == 100);
	
	assert(lpconfig_get_retransmit_request_interval()->tv_sec == 1); 
	assert(lpconfig_get_retransmit_request_interval()->tv_usec == 0);
	// assert(lpconfig_get_socket_buffers_size() == 2097152); 
	
		
    lpconfig_destroy();
    
	printf("TEST SUCCESSFUL!\n");
    return 0;
}
