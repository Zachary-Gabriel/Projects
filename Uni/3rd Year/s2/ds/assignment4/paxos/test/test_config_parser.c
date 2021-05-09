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
    result = config_mngr_init("./etc/config1.cfg", acc_id, NULL);

    assert(result == 0);

    assert(lpconfig_get_p1_interval()->tv_sec == 1);
    assert(lpconfig_get_p1_interval()->tv_usec == 0);
    assert(lpconfig_get_p2_interval()->tv_sec == 0);
    assert(lpconfig_get_p2_interval()->tv_usec == 100000);

    assert(lpconfig_get_acceptors_count() == 3);
    
    assert(strcmp("192.168.1.2", lpconfig_get_ring_inbound_addr()) == 0);
    assert(lpconfig_get_ring_inbound_port() == 1235);
    assert(lpconfig_get_learners_inbound_port() == 5552);
    
    assert(strcmp("192.168.1.1", lpconfig_get_ip_addr_of(1)) == 0);
    assert(lpconfig_get_ring_port_of(1) == 1234);
    assert(lpconfig_get_learners_port_of(1) == 5551);

    lpconfig_destroy();
    
    //Invalid configurations test
    char config_filename[100];
    int i;
    for(i = 1; i < 8; i++) {
        sprintf(config_filename, "./etc/config-error%d.cfg", i);
        printf("\n >>> Testing configuration %s\n", config_filename);
        result = config_mngr_init(config_filename, acc_id, NULL);
        assert(result == -1);
    }
    
    return 0;
}
