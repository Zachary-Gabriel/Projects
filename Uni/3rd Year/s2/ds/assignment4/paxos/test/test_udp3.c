#include <unistd.h>
#include <stdlib.h>
#include "forced_assert.h"

#include <event.h>

#include "lp_utils.h"
#include "lp_config_parser.h"
#include "lp_network.h"
#include "test_header.h"

int main (int argc, char const *argv[]) {

    UNUSED_ARG(argc);
    UNUSED_ARG(argv);
    
    int result = -1;
    acceptor_id_t acc_id;
    
    int port = 6666;

    // Valid config
    acc_id = 2;
    result = config_mngr_init("./etc/config1.cfg", acc_id, NULL);
    assert(result == 0);

    event_init();


    //Create senders with invalid arguments, 
    // constructor should return NULL
    udp_sender * us;

    us = udp_sender_init("127.0.0.1.0", port);
    assert(us == NULL);

    us = udp_sender_init("300.0.0.1", port);
    assert(us == NULL);

    us = udp_sender_init("0.0.1", port);
    assert(us == NULL);
    
    printf("Test completed\n");
    exit(0);

    return 0;
}
