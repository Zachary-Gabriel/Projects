#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include <event.h>

#include "lp_network.h"
#include "lp_timers.h"
#include "lp_config_parser.h"
#include "lp_utils.h"

// Defined in network_common.c
void socket_set_reuse_port(int sock);
void socket_set_nonblocking(int sock);
void socket_set_bufsize(int sock, int size);

/*************************************************
   UDP Receiver 
*************************************************/

struct udp_receiver_t {
    bool is_multicast;
    char ip_str[16];
    int port;
    handle_datagram_cb cb;
    struct event recv_event;
    
    int sock;
    struct sockaddr_in saddr;
    char * recv_buf;
    int recv_buf_size;

    receive_cb post_recv_cb;
    receive_cb pre_recv_cb;
	receive_cb post_dlvr_cb;

	//Bandwidth Statistics
	long unsigned bytes_received;
	int last_print_time;
	long unsigned msg_received;
	long unsigned tot_msg_received;
    
};

static void
udp_read_callback_wrapper(int fd, short event, void *arg)
{
    UNUSED_ARG(event);
    
    udp_receiver * ur = arg;
    int datasize;
    
    //Invoke the pre-receive callback (if set)
    if(ur->pre_recv_cb != NULL) {
        ur->pre_recv_cb();
    }

    //Get the message
    socklen_t addrlen = sizeof(struct sockaddr);
    datasize = recvfrom(fd,                 //Socket to read from
        ur->recv_buf,                       //Where to store the msg
        ur->recv_buf_size, 					//Size of buffer
        MSG_WAITALL,                        //Get the entire message
        (struct sockaddr *)&ur->saddr,      //Address
        &addrlen);                          //Address length

    //Error in recvfrom
    if (datasize < 0) {
        perror("recvfrom");
        return;
    }

#ifdef UDP_STATS
	ur->bytes_received += datasize;
	ur->msg_received += 1;
#endif

    //Invoke the post-receive callback (if set)
    if(ur->post_recv_cb != NULL) {
        ur->post_recv_cb();
    }
    
    uint16_t udatasize = (uint16_t)datasize;
    //A packet may contain different messages, 
    // invoke the receiving callback for each one
    uint16_t current_offset = 0;
    lp_message_header * next_msg;    
    while(current_offset < udatasize) {
        next_msg = (lp_message_header *)&ur->recv_buf[current_offset];
        
        //This message has invalid size, to avoid strange behavior, 
        // drop the entire packet.
        if(current_offset + sizeof(lp_message_header) + next_msg->size > udatasize) {
            LOG_MSG(WARNING, ("WARNING: corrupted message detected [%d bytes], dropping this packet\n",
             next_msg->size));
            break;
        }

#ifdef MESSAGE_LOSS_PROBABILITY
		//If the above symbol is defined, simulate random packet loss
		if((random() % 100) < MESSAGE_LOSS_PROBABILITY) {
			//Packet is "lost"
			LOG_MSG(WARNING, ("Voluntarily dropping a message (type:%d)\n", next_msg->type));
		} else {
			//Packet is not "lost"
			LOG_MSG(NETWORK, ("Delivering %u bytes of network data (type:%d)\n", 
				next_msg->size, next_msg->type));
	        ur->cb(&next_msg->data, next_msg->size, next_msg->type);
		}
#else
		//Normal case, no (voluntary) packet loss
		LOG_MSG(NETWORK, ("Delivering %u bytes of network data (type:%d)\n", 
			next_msg->size, next_msg->type));
        ur->cb(&next_msg->data, next_msg->size, next_msg->type);
#endif     
		//Move cursor forward to next message in packet
        current_offset += (next_msg->size + sizeof(lp_message_header));
    }

    //Invoke the post-deliver callback (if set)
    if(ur->post_dlvr_cb != NULL) {
        ur->post_dlvr_cb();
    }
}

udp_receiver *
udp_receiver_init(
    char* addr_str,       /*Listen address (optional)*/
    int port,             /*Listen port*/
    handle_datagram_cb cb /*Callback for packet received*/
    )
    {
	
#ifdef MESSAGE_LOSS_PROBABILITY
		//Init random seed if the above symbol is defined
		//(Seeding happens multiple times but it's not relevant...);
		LOG_MSG(WARNING, ("WARNING: Voluntarily dropping messages with probability %d%%\n", MESSAGE_LOSS_PROBABILITY));
		srandom(time(NULL));
#endif
	
	
        LOG_MSG(DEBUG, ("Creating receiver for UDP port %d\n", port));
        
        // Create struct to return
        udp_receiver * ur = NULL;
        ur = calloc(1, sizeof(udp_receiver));
        assert(ur != NULL);
        
        //Copy the IP address string
        if(addr_str != NULL) {            
            assert(strlen(addr_str) < 16);
            memcpy(ur->ip_str, addr_str, strlen(addr_str));
            ur->ip_str[strlen(addr_str)] = '\0';
        }

        //Save the callback invoked when data is received
        ur->cb = cb;
        
        //Get buffer size and allocate it
        ur->recv_buf_size = MAX_UDP_PAYLOAD;
        ur->recv_buf = calloc(1, ur->recv_buf_size);
        
        // Create socket and set options
        ur->sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (ur->sock < 0) {
            perror("socket creation");
            return NULL;
        }
        socket_set_reuse_port(ur->sock);
        socket_set_nonblocking(ur->sock);
		if(lpconfig_get_socket_buffers_size() > 0) {
	        socket_set_bufsize(ur->sock, lpconfig_get_socket_buffers_size());			
		}

        // Configure address and bind
        bzero(&ur->saddr, sizeof(struct sockaddr_in));
        ur->saddr.sin_family = AF_INET;
        // If addr_str is non-null, it can be used to bind a specific interface
        ur->saddr.sin_addr.s_addr = INADDR_ANY;
        ur->saddr.sin_port = htons(port);
        if (bind(ur->sock, (struct sockaddr *)&ur->saddr, sizeof(struct sockaddr_in)) < 0) { 
            perror("bind");
            return NULL;
        }

        // Create receive event
        event_set(&ur->recv_event, ur->sock, EV_READ|EV_PERSIST, udp_read_callback_wrapper, ur);
        event_add(&ur->recv_event, NULL);

	//Bandwidth Statistics
		ur->bytes_received = 0;
		ur->msg_received = 0;
		ur->tot_msg_received = 0;
		ur->last_print_time = time(NULL);
        
        LOG_MSG(INFO, ("Created receiver for UDP port %d\n", port));
        return ur;
    }

udp_receiver *
	mcast_receiver_init(
	char* addr_str,       /*Listen address*/
	int port,             /*Listen port*/
	handle_datagram_cb cb /*Callback for packet received*/
	)
{
			//addr_str cannot be null, unlike normal udp receiver
	if(addr_str == NULL) {
		printf("Error: NULL multicast address\n");
		return NULL;
	}
	LOG_MSG(DEBUG, ("Creating receiver for multicast address %s:%d\n", addr_str, port));

			//Create a normal udp receiver
	udp_receiver * ur = udp_receiver_init(addr_str, port, cb);
	if(ur == NULL) {
		return NULL;
	}
	ur->is_multicast = true;

			// Set up membership to multicast group 
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(addr_str);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(ur->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) != 0) {
		perror("setsockopt, setting IP_ADD_MEMBERSHIP");
				// udp_receiver_destroy(ur);
		return NULL;
	}

	LOG_MSG(INFO, ("Created receiver for multicast address %s:%d\n", addr_str, port));
	return ur;
}

void udp_receiver_set_preread_callback(udp_receiver * ur, receive_cb cb) {
    ur->pre_recv_cb = cb;
}


void udp_receiver_set_postread_callback(udp_receiver * ur, receive_cb cb) {
    ur->post_recv_cb = cb;
}

void udp_receiver_set_postdeliver_callback(udp_receiver * ur, receive_cb cb) {
    ur->post_dlvr_cb = cb;
}

void udp_receiver_print_stats(udp_receiver * ur, int current_time) {
#ifndef UDP_STATS
	return;
#endif

	int elapsed_time = current_time - ur->last_print_time;
	
	if(elapsed_time == 0 || ur->msg_received == 0 ) {
		printf("--\n");
		return;
	}
	
	long unsigned Mbps = ((ur->bytes_received * 8) / (1024*1024)) / elapsed_time;
	long unsigned avg_pack_size_kb = (ur->bytes_received / 1024 / ur->msg_received);
	
	printf("  Recv %lu Mbit/s (%d sec) Avg %lu Kbytes/packet\n", 
		Mbps, elapsed_time, avg_pack_size_kb);

	ur->tot_msg_received += ur->msg_received;
	ur->bytes_received = 0;
	ur->msg_received = 0;
	ur->last_print_time = current_time;
	
	printf("  %lu message received\n", ur->tot_msg_received);
}

/*************************************************
   UDP Sender
*************************************************/

struct udp_sender_t {
	bool is_multicast;
    char ip_str[16];
    int port;
    
    int sock;
    struct sockaddr_in saddr;
    char * send_buf;
    int send_buf_size;
    int current_buf_size;
	periodic_event * autoflush_event;
	
	//Bandwidth Statistics
	long unsigned bytes_sent;
	int last_print_time;
	long unsigned msg_sent;
	long unsigned tot_msg_sent;
};

udp_sender *
udp_sender_init(
    char* addr_str,       /*Remote address string*/
    int port              /*Remote port*/
    )
    {
	
		assert(MAX_MESSAGE_SIZE <= (MAX_UDP_PAYLOAD+sizeof(lp_message_header)));
        LOG_MSG(DEBUG, ("Creating sender for UDP address:%s port:%d\n", addr_str, port));
        
        // Create struct to return
        udp_sender * us = NULL;
        us = calloc(1, sizeof(udp_sender));
        assert(us != NULL);
        us->send_buf = NULL;
		us->is_multicast = false;
        
        
        //Copy the IP address string
        assert(addr_str != NULL);
        if (strlen(addr_str) >= 16 || strlen(addr_str) < 7) {
            printf("Error: Malformed address %s\n", addr_str);
            goto udp_sender_init_error;
        }
        
        //Save remote IP
        memcpy(us->ip_str, addr_str, strlen(addr_str));
        us->ip_str[strlen(addr_str)] = '\0';
                
        //Get buffer size and allocate it
        us->current_buf_size = 0;
        us->send_buf_size = MAX_UDP_PAYLOAD;
        us->send_buf = calloc(1, us->send_buf_size);

        // Create socket and set options
        us->sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (us->sock < 0) {
            perror("socket creation");
            goto udp_sender_init_error;
        }
        socket_set_reuse_port(us->sock);
        socket_set_nonblocking(us->sock);
		if(lpconfig_get_socket_buffers_size()) {
			socket_set_bufsize(us->sock, lpconfig_get_socket_buffers_size());			
		}

        // Configure address and connect
        bzero(&us->saddr, sizeof(struct sockaddr_in));
        us->saddr.sin_family = AF_INET;
        us->saddr.sin_addr.s_addr = inet_addr(us->ip_str);
        if (us->saddr.sin_addr.s_addr == INADDR_NONE) {
            printf("Error: Invalid address %s\n", us->ip_str);
            goto udp_sender_init_error;
        }
        
        us->saddr.sin_port = htons(port);
        if (connect(us->sock, (struct sockaddr *)&us->saddr, sizeof(struct sockaddr_in)) < 0) { 
            perror("connect");
            goto udp_sender_init_error;
        }

		//Stats		
		us->bytes_sent = 0;
		us->msg_sent = 0;
		us->last_print_time = time(NULL);
		us->tot_msg_sent = 0;
        
        LOG_MSG(INFO, ("Created sender for UDP address %s:%d\n", us->ip_str, port));
        return us;
 
       
udp_sender_init_error:
    if (us->send_buf != NULL) {
        free(us->send_buf);
    }
    free(us);
    return NULL;
}

udp_sender *
	mcast_sender_init(
	char* addr_str,       /*Listen address*/
	int port             /*Listen port*/
	)
{
			//addr_str cannot be null, unlike normal udp receiver
	if(addr_str == NULL) {
		printf("Error: NULL multicast address\n");
		return NULL;
	}
	LOG_MSG(DEBUG, ("Creating sender for multicast address %s:%d\n", addr_str, port));

			//Create a normal udp receiver
	udp_sender * us = udp_sender_init(addr_str, port);
	if(us == NULL) {
		return NULL;
	}
	us->is_multicast = true;

	// Set up membership to multicast group 
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(addr_str);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(us->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) != 0) {
		perror("setsockopt, setting IP_ADD_MEMBERSHIP");
				// udp_receiver_destroy(ur);
		return NULL;
	}

	LOG_MSG(INFO, ("Created sender for multicast address %s:%d\n", addr_str, port));
	return us;
}

int 
udp_sender_force_flush(udp_sender * us) {
    assert(us->current_buf_size <= us->send_buf_size);
    
    //Nothing to send
    if(us->current_buf_size == 0) {
        return 0;
    }
    
    int data_sent = send(us->sock, us->send_buf, us->current_buf_size, 0);
    if(data_sent < 0) {
        perror("send");
		return 1;
    }
	LOG_MSG(NETWORK, ("Send buffer flushed,  %u bytes\n", data_sent));
#ifdef UDP_STATS
	us->bytes_sent += data_sent;
	us->msg_sent += 1;
#endif
    
    //TODO if this get triggered, add further header to each packet 
    // with total data size (so that incomplete packets can be discarded)
	LOG_MSG_COND(NETWORK, (data_sent != us->current_buf_size), 
		("Sent only %u of %u bytes\n", data_sent, us->current_buf_size));
    assert(data_sent == us->current_buf_size);
    
    us->current_buf_size = 0;
    return 0;
}

void 
net_send_udp(udp_sender * us, void * data, int size, lp_msg_type type) {
	
	if(size > MAX_MESSAGE_SIZE) {
		printf("Error: cannot send packets of size %d (Max is %d)\n",
			size, MAX_MESSAGE_SIZE);
		return;
	}

    int total_size = size + sizeof(lp_message_header);
	
    //If data does not fit in the current buffer, flush it.
    if(us->current_buf_size + total_size > MAX_MESSAGE_SIZE) {
        udp_sender_force_flush(us);
    }
    
	LOG_MSG(NETWORK, ("Adding %u (+%lu) bytes to send buffer\n", 
		size, sizeof(lp_message_header)));
    
    //When batching, each message is preceeded by its size
    lp_message_header * destination = (lp_message_header *)&us->send_buf[us->current_buf_size];
    destination->size = size;
    destination->type = type;
    
    //Copy the message data
    memcpy(destination->data, data, size);
    
    //Update current buffer size
    us->current_buf_size += total_size;

    //If there's no space left in the current buffer, flush it.
    if((MAX_MESSAGE_SIZE - us->current_buf_size) <= (int)sizeof(lp_message_header)) {
        udp_sender_force_flush(us);
    }
}

void udp_autoflush_cb(void * us) {
	udp_sender_force_flush((udp_sender*)us);
}

void udp_sender_enable_autoflush(udp_sender * us, unsigned milliseconds) {
	struct timeval interval;
	interval.tv_sec = (milliseconds/1000);
	interval.tv_usec = (milliseconds % 1000) * 1000;
	us->autoflush_event = set_periodic_event(&interval, udp_autoflush_cb, us);
}

void udp_sender_enable_default_autoflush(udp_sender * us) {
	udp_sender_enable_autoflush(us, lpconfig_get_default_autoflush_interval());
}

void udp_sender_print_stats(udp_sender * us, int current_time) {
#ifndef UDP_STATS
	return;
#endif

	int elapsed_time = current_time - us->last_print_time;
	if(elapsed_time == 0 || us->msg_sent == 0) {
		printf("--\n");
		return;
	}

	long unsigned Mbps = ((us->bytes_sent * 8) / (1024*1024)) / elapsed_time;
	long unsigned avg_pack_size_kb = (us->bytes_sent / 1024 / us->msg_sent);
	
	printf("  Send %lu Mbit/s (%d sec), Avg %lu Kbytes/packet\n", 
		Mbps, elapsed_time, avg_pack_size_kb);

	us->tot_msg_sent += us->msg_sent;
	us->bytes_sent = 0;
	us->msg_sent = 0;
	us->last_print_time = current_time;
	
	printf("  %lu messages sent\n", us->tot_msg_sent);
}
