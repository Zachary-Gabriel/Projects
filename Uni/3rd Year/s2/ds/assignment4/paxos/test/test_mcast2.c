#include <unistd.h>
#include <stdlib.h>
#include "forced_assert.h"

#include <event.h>

#include "lp_utils.h"
#include "lp_config_parser.h"
#include "lp_network.h"
#include "test_header.h"

void handle_msg(void* data, size_t datasize, lp_msg_type type) {
    UNUSED_ARG(data);
    UNUSED_ARG(datasize);
    UNUSED_ARG(type);
}

int main (int argc, char const *argv[]) {

    UNUSED_ARG(argc);
    UNUSED_ARG(argv);
    
    int result = -1;
    acceptor_id_t acc_id;
    
    // Valid config
    acc_id = 2;
    result = config_mngr_init("./etc/config1.cfg", acc_id, NULL);
    assert(result == 0);

    event_init();


    //Create senders with invalid arguments, 
    // constructor should return NULL
    udp_receiver * ur;

    ur = mcast_receiver_init("127.0.0.1", 1111, handle_msg);
    assert(ur == NULL);

    ur = mcast_receiver_init("300.0.0.1", 1112, handle_msg);
    assert(ur == NULL);

    ur = mcast_receiver_init("0.0.1", 1113, handle_msg);
    assert(ur == NULL);

    ur = mcast_receiver_init("239.0.0.1", 22, handle_msg);
    assert(ur == NULL);

    ur = mcast_receiver_init("239.0.0.1", 1114, handle_msg);
    assert(ur != NULL);

    ur = mcast_receiver_init("224.0.0.1", 1115, handle_msg);
    assert(ur != NULL);

    
    printf("Test completed\n");
    exit(0);

    return 0;
}
