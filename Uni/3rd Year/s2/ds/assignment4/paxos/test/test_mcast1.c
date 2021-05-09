#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "forced_assert.h"
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>

#include <event.h>

#include "lp_utils.h"
#include "lp_config_parser.h"
#include "lp_network.h"
#include "lp_timers.h"
#include "test_header.h"

static char * msgs[3] = {
    "This is message 0, sent alone in a packet.",
    "Message 1 is sent later...", 
    "...And should arrive in a batch with Message 2."
};

static lp_msg_type msgs_types[3] = {
    test1,
    test2, 
    test3
};

static bool received[3] = {false, false, false};
static int timeout_count = 0;

void timeout_check(void * arg) {
    UNUSED_ARG(arg);
    timeout_count += 1;
    printf("waiting....\n");
    
    if(timeout_count > 5) {
        printf("No data received, exiting\n");
        printf("******************************\n");
        printf("***        IMPORTANT       ***\n");
        printf("******************************\n");
        printf("This test sometimes fails on Linux, ");
        printf("probably just on systems with multiple active NICs\n");
        printf("( For example the head node of a cluster)\n");
        exit(1);
    }
}

void send_data(void * arg) {
    UNUSED_ARG(arg);
    udp_sender * us = arg;
    
    //First message goes alone (force with flush)
    net_send_udp(us, msgs[0], strlen(msgs[0]), msgs_types[0]);
    udp_sender_force_flush(us);

    //Second and third should be batched
    net_send_udp(us, msgs[1], strlen(msgs[1]), msgs_types[1]);
    net_send_udp(us, msgs[2], strlen(msgs[2]), msgs_types[2]);
    udp_sender_force_flush(us);
    
}

void handle_msg(void* data, size_t datasize, lp_msg_type type) {
    
    unsigned int i;
    for(i = 0; i < 3; i++) {
        
        //Type does not match
        if(msgs_types[i] != type) 
            continue;
        
        //Size does not match
        if(strlen(msgs[i]) != datasize) 
            continue;
            
        //String does not match
        if(strncmp(msgs[i], data, datasize) != 0)
            continue;
        
        //Match found!
        printf("Received message %d\n", (int)i);
        received[i] = true;
        break;
    }
    
    if(received[0] && received[1] && received[2]) {
        printf("All messages received!\n");
        exit(0);
    }
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

    int port = lpconfig_get_mcast_port();
    char * mcast_addr = lpconfig_get_mcast_addr();

    event_init();
    
    udp_receiver * ur = mcast_receiver_init(mcast_addr, port, handle_msg);
    assert(ur != NULL);

    udp_sender * us = mcast_sender_init(mcast_addr, port);
    assert(us != NULL);

    struct timeval recv_check;
    recv_check.tv_sec = 1;
    recv_check.tv_usec = 0;
    set_periodic_event(&recv_check, timeout_check, NULL);

    struct timeval send_interval;
    send_interval.tv_sec = 0;
    send_interval.tv_usec = 500000;
    set_periodic_event(&send_interval, send_data, us);

    //Infinite event loop, will exit on message received
    event_dispatch();


    return 0;
}
