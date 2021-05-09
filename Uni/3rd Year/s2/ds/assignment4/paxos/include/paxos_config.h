#ifndef PAXOS_CONFIG_H_H2ZHN8AC
#define PAXOS_CONFIG_H_H2ZHN8AC

#include <stdint.h>

typedef uint8_t acceptor_id_t;
typedef uint32_t ballot_t;
typedef long unsigned iid_t;

typedef struct command_id_t {
    uint8_t mcaster_id;
    uint8_t mcaster_incarnation;
    uint16_t cmd_seqnum;
} command_id;

#define MAX_TCP_PAYLOAD (9000-16) //Multiple of MTU - tcp header
#define MAX_UDP_PAYLOAD (9000-12) //Multiple of MTU - largest udp header+pseudoheader
#define MAX_MESSAGE_SIZE (MAX_TCP_PAYLOAD - 4) //Remove sizeof(lp_message_header)

#define MAX_ACCEPTORS 10

#define MAX_QUEUE_SIZE_BYTES (1024u*1024u*1024u) // 1GB

/*
	The following defines the verbosity level,
	individual modules can be enabled selectively by ORing them, 
	See lp_utils.h for the complete list of switches
	Examples:
	#define LP_VERBOSITY (WARNING|INFO|PAXOS|PAXOS_DBG)
	#define LP_VERBOSITY (LOG_ALL)
	#define LP_VERBOSITY (LOG_NONE)
	#define LP_VERBOSITY (WARNING|DELIVERY_Q|MCAST_STORE|ACC_STORE)
*/
#define LP_VERBOSITY (WARNING)

/*
	Same as logging/verbosity but those are event counters
*/
#define LP_EVENTCOUNTERS (LOG_ALL)

/*
	Interval for printing event counters {seconds, microseconds}
	Leave defined even if no counter is enabled (LOG_NONE)
*/
#define LP_COUNTERS_PRINT_INTERVAL {10, 0}

/*
	Uncomment the following for debugging purposes, 
	all (UDP/MULTICAST) network receivers will volountarily drop some
	incoming messages with the given probability (set between 0 and 99)
*/
// #define MESSAGE_LOSS_PROBABILITY 5


#endif /* end of include guard: PAXOS_CONFIG_H_H2ZHN8AC */
