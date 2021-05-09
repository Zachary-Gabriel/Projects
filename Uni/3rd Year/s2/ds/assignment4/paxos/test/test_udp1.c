#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "forced_assert.h"
#include <sys/time.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <event.h>

#include "lp_utils.h"
#include "lp_config_parser.h"
#include "lp_network.h"
#include "lp_timers.h"
#include "test_header.h"

char * my_string = "This string should NOT be received!";

int child_return_value = 0;

void handle_message(void* data, size_t datasize, lp_msg_type type) {

    UNUSED_ARG(type);
    printf("Error: some data was received that should have been dropped!\n");
    printf("[%d bytes] %s\n", (int)datasize, (char*)data);
    exit(1);
}

static int timeout_count = 0;
void timeout_check(void * arg) {
    UNUSED_ARG(arg);
    timeout_count += 1;
    printf("waiting data....\n");
    
    if(timeout_count > 5) {
        printf("No data received, waiting child termination\n");

		int pid = wait(&child_return_value);
		if(pid <= 0) {
			if(errno == ECHILD) {
				printf("Child terminated earlier\n");
				exit(0);
			} else {
				perror("wait");
				exit(1);
			}
		} else if(child_return_value != 0) {
			printf("Child exited with error\n");
			exit(1);
		} else {
			printf("TEST SUCCESSFUL!\n");
			exit(0);
	        
		}
    }
}

int make_socket(char * addr, int port) {
	int sock;
		// Create socket and set options
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket creation");
		exit(1);
	}

	// Configure address and connect
	struct sockaddr_in saddr;
	bzero(&saddr, sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(addr);
	if (saddr.sin_addr.s_addr == INADDR_NONE) {
		printf("Error: Invalid address %s\n", addr);
		exit(1);
	}

	saddr.sin_port = htons(port);
	if (connect(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) < 0) { 
		perror("connect");
		exit(1);
	}
	
	printf("Socket created successfully\n");
	return sock;
}

int main (int argc, char const *argv[]) {


    UNUSED_ARG(argc);
    UNUSED_ARG(argv);
    
    int port = 6666;
    
    int result = -1;
    acceptor_id_t acc_id;
    
    // Valid config
    acc_id = 2;
    result = config_mngr_init("./etc/config1.cfg", acc_id, NULL);
    assert(result == 0);
    
    int child_pid = fork();
    assert(child_pid >= 0);

    if(child_pid == 0) {
        
        // Child only sends some data to parent, then exits
        sleep(1);
		int sock = make_socket("127.0.0.1", port);
		
		int data_sent;
		unsigned i;
		for(i = 0; i < 5; ++i) {
			data_sent = send(sock, my_string, strlen(my_string), 0);
		    if(data_sent < 0) {
		        perror("send");
				exit(1);
		    }
			sleep(1);
		}
		printf("All messages sent\n");
        exit(0);
        
    } else {
        event_init();
        // Child process creates a receiver and waits for a message
        udp_receiver * ur = udp_receiver_init(NULL, port, handle_message);
        assert(ur != NULL);
        
        struct timeval recv_check;
        recv_check.tv_sec = 1;
        recv_check.tv_usec = 0;
        set_periodic_event(&recv_check, timeout_check, NULL);
        
        //Infinite event loop, will exit on message received
        event_dispatch();
        
    }
    
    
    
    return 0;
}

