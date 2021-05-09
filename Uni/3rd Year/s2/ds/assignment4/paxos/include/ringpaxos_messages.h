#include <stdbool.h>

// Message types used in ring-paxos

typedef struct phase1_msg_t {
    iid_t inst_number;
    ballot_t ballot;
    unsigned promises_count;
    ballot_t highest_accepted_ballot;
    ballot_t highest_promised_ballot;
    command_id cmd_key;
    size_t cmd_size;
    char cmd_value[0];
} phase1_msg;
#define PH1_MSG_SIZE(M) (sizeof(phase1_msg) + M->cmd_size)
#define PH1_MSG_SIZE_S(M) (sizeof(phase1_msg) + M.cmd_size)

typedef struct phase1_range_msg_t {
	iid_t from;	
	iid_t to;	
	ballot_t ballot;	
	unsigned promises_count;
} phase1_range_msg;

typedef struct phase2_msg_t {
    iid_t inst_number;
    ballot_t ballot;
    unsigned accepts_count;
    command_id cmd_key;
} phase2_msg;

typedef struct cmdmap_msg_t {
    iid_t inst_number;
    command_id cmd_key;
    size_t cmd_size;
    char cmd_value[0];
} cmdmap_msg;
#define CMDMAP_MSG_SIZE(M) (sizeof(cmdmap_msg) + M->cmd_size)

typedef struct acceptance_msg_t {
    iid_t inst_number;
    command_id cmd_key;
} acceptance_msg;

typedef struct chosencmd_requests_msg_t {
	unsigned requests_count;
	iid_t inst_number[0];
} chosencmd_requests_msg;
#define FINVAL_REQS_MSG_SIZE(M) (sizeof(chosencmd_requests_msg) + (M->requests_count*sizeof(iid_t)))


typedef struct map_requests_msg_t {
	unsigned requests_count;
	iid_t inst_number[0];
} map_requests_msg;
#define CMDMAP_REQS_MSG_SIZE(M) (sizeof(map_requests_msg) + (M->requests_count*sizeof(iid_t)))

#define REPEAT_REQUEST_MAX_ENTRIES (((MAX_MESSAGE_SIZE - sizeof(map_requests_msg)) / sizeof(iid_t)) -1)

#define MAX_COMMAND_SIZE (MAX_MESSAGE_SIZE - sizeof(phase1_msg))
