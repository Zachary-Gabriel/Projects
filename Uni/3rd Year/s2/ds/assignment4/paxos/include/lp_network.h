#include "paxos_config.h"

// The following objects take care of sending/receiving network data

#define UDP_STATS //Enable UDP senders/receivers statistics

typedef enum lp_msg_type_e {
    // Ring Paxos
    phase1=1,
	phase1_range=2,
    phase2=3,
    command_map=4,
    refusal=5,
    acceptance=6,
	map_request=7,
	chosenval_request=8,
	client_submit=9,
    
    /*...*/
    
    test1=512,
    test2=513,
    test3=514,
    notype=0
} lp_msg_type;

// Header appended to each message in a packet
// (multiple messages can be sent in the same packet)
typedef struct lp_message_header_t {
    uint16_t size;
    lp_msg_type type;
    char data[0];
}lp_message_header;

/*
UDP Receiver 
*/
struct udp_receiver_t;
typedef struct udp_receiver_t udp_receiver;

//Callback invoked for each message in a packet
typedef void(*handle_datagram_cb)(void*, size_t, lp_msg_type);


//Creates a new receiver object for UDP
udp_receiver *
udp_receiver_init(
    char* addr_str,       /*Listen address (optional)*/
    int port,             /*Listen port*/
    handle_datagram_cb cb /*Callback for packet received*/
    );

//Creates a new receiver object for IP-Multicast
udp_receiver *
mcast_receiver_init(
    char* addr_str,       /*Listen address*/
    int port,             /*Listen port*/
    handle_datagram_cb cb /*Callback for packet received*/
    );

//The following set receive_cb callbacks at specific points of the receiver code
typedef void(*receive_cb)(void);

//Invoked before performing a 'read' systemcall
void udp_receiver_set_preread_callback(udp_receiver * ur, receive_cb cb);
//Invoked immediately after performing a 'read' systemcall
void udp_receiver_set_postread_callback(udp_receiver * ur, receive_cb cb);
//Invoked after delivering all messages in the packet
void udp_receiver_set_postdeliver_callback(udp_receiver * ur, receive_cb cb);

//Prints bandwidth statistics for this receiver
void udp_receiver_print_stats(udp_receiver * ur, int current_time);


/*
UDP Sender 
*/
struct udp_sender_t;
typedef struct udp_sender_t udp_sender;

//Creates a new sender object for UDP
udp_sender *
udp_sender_init(
    char* addr_str,       /*Remote address string*/
    int port              /*Remote port*/
    );

//Creates a new sender object for IP-Multicast
udp_sender *
mcast_sender_init(
    char* addr_str,       /*Remote address string*/
    int port              /*Remote port*/
    );

//Buffered send, appends the message to the current packet
//If it does not fit, the current packet is flushed.
void net_send_udp(udp_sender * us, void * data, int size, lp_msg_type type);
//Immediately flush the current packet
int udp_sender_force_flush(udp_sender * s);
//Enable automatic, periodic flushing of packets
void udp_sender_enable_autoflush(udp_sender * us, unsigned milliseconds);
//Enable automatic flushing with interval defined in the configuration file
void udp_sender_enable_default_autoflush(udp_sender * us);

//Prints bandwidth statistics for this sender
void udp_sender_print_stats(udp_sender * us, int current_time);

/*
TCP Client
*/

// struct tpc_connection_t;
// struct struct tpc_connection_t tpc_connection;
// typedef void(* handle_tpc_error_cb)(tpc_connection*);
// 
// tcp_connection * 
// tcp_connection_init(
//     char* addr_str,       /*Destination address*/
//     int port,             /*Destination port*/
//     handle_datagram_cb data_cb /*Callback for data received*/
//     handle_tpc_error_cb err_cb /*Callback for disconnection*/
// );


/*
TCP Server
*/
