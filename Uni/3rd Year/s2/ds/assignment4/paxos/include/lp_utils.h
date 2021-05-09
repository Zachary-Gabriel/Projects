#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "paxos_config.h"

void print_cmdline_args(int argc, char const *argv[]);
void validate_acceptor_args_or_quit(int argc, char const *argv[]);
void enable_ctrl_c_handler();

#define UNUSED_ARG(A) ( A = A )

ballot_t make_first_ballot(acceptor_id_t acc_id, unsigned incarnation);
ballot_t raise_ballot(ballot_t current_b, ballot_t ballot_to_beat);
ballot_t increment_ballot(ballot_t b);
bool is_my_ballot(ballot_t b, acceptor_id_t acc_id, unsigned incarnation);

void clear_cmd_key(command_id * cmd_key);
char * print_cmd_key(command_id * cmd_key);

#define IID_MAX(X, Y) (X > Y ? X : Y)
#define TIMEVAL_MIN(TV1, TV2) ((TV1->tv_sec < TV2->tv_sec) || (TV1->tv_sec == TV2->tv_sec && TV1->tv_usec < TV2->tv_usec)   ? TV1 : TV2)
#define TIMEVAL_IS_GT(TV1, TV2) ((TV1->tv_sec > TV2->tv_sec) || (TV1->tv_sec == TV2->tv_sec && TV1->tv_usec > TV2->tv_usec))


//Equality test for command_id
#define CMD_KEY_EQUALS(X, Y) (X->mcaster_id == Y->mcaster_id && X->mcaster_incarnation == Y->mcaster_incarnation && X->cmd_seqnum == Y->cmd_seqnum)
//Copy key y into key x
#define CMD_KEY_COPY(X, Y) (memcpy((void*)X, (void*)Y, sizeof(command_id)))

#define CMD_KEY_IS_NULL(X) (X->mcaster_id == 0 && X->mcaster_incarnation == 0 && X->cmd_seqnum == 0)
#define CMD_KEY_CLEAR(X) {X->mcaster_id = 0; X->mcaster_incarnation = 0; X->cmd_seqnum = 0;}

/****************************************************************************
// LOGGING MACROS	
****************************************************************************/

#ifdef WARNING
#error error: WARNING is already defined
#endif
#define WARNING 1

#ifdef INFO
#error error: INFO is already defined
#endif
#define INFO (1<<1)

#ifdef DEBUG
#error error: DEBUG is already defined
#endif
#define DEBUG (1<<2)

#ifdef PAXOS
#error error: PAXOS is already defined
#endif
#define PAXOS (1<<3)

#ifdef PAXOS_DBG 
#error error: PAXOS_DBG is already defined
#endif
#define PAXOS_DBG (1<<4)

#ifdef NETWORK
#error error: NETWORK is already defined
#endif
#define NETWORK (1<<11)

#ifdef DELIVERY_Q
#error error: DELIVERY_Q is already defined
#endif
#define DELIVERY_Q (1<<12)

#ifdef MCAST_STORE
#error error: MCAST_STORE is already defined
#endif
#define MCAST_STORE (1<<13)

#ifdef ACC_STORE
#error error: ACC_STORE is already defined
#endif
#define ACC_STORE (1<<14)

#ifdef TIMERS
#error error: TIMERS is already defined
#endif
#define TIMERS (1<<15)

#ifdef LOG_ALL
#error error: LOG_ALL is already defined
#endif
#define LOG_ALL (0xFFFF)

#ifdef LOG_NONE
#error error: LOG_NONE is already defined
#endif
#define LOG_NONE (0x0000)

#define LOG_MSG(L, S) {if(L & LP_VERBOSITY) {\
   printf("[%s] ", __func__) ;\
   printf S ;\
}}

#define LOG_MSG_COND(L, C, S) {if((LP_VERBOSITY & L) && (C)) {\
   printf("[%s] ", __func__) ;\
   printf S ;\
}}


/****************************************************************************
// EVENT COUNTING MACROS
****************************************************************************/
#define COUNT_EVENT(L, E) {if(L & LP_EVENTCOUNTERS) { E += 1; }}
#define PRINT_COUNT(NAME) {printf(#NAME":%lu\n", NAME);}
#define COUNT_EVENT_DISTRIBUTION(A, I) {A[I] += 1;}
#define PRINT_COUNT_DISTRIBUTION(A, M) {\
	unsigned i;							\
	printf(#A":"); 						\
	for(i = 0; i <= M; ++i) { 			\
		printf(" %u:[%u]", i, A[i]);	\
	}									\
	printf("\n");						\
}
