#include <stdlib.h>

#include "paxos_config.h"
#include "lp_clients_handling.h"

// This module is used by the Paxos leader to store information
// relative to the currently open/in-progress instances

typedef enum mcaster_instance_status_e {
    empty=0,
    ready=1,
    p1_pending=2,
	p1_done=3,
    p2_pending=4,
    done=5
} mcaster_instance_status;

typedef struct mcaster_instance_record_t {
    iid_t inst_number;
    mcaster_instance_status status;
    ballot_t ballot;
    command_id assigned_cmd_key;
    void * assigned_cmd_value;
    size_t assigned_cmd_size;
    struct timeval timeout;
	struct timeval repeat_cmdmap_timeout;
} mcaster_instance_record;

typedef struct mcaster_storage_mngr_t mcaster_storage_mngr;


//Initialization of leader storage manager
mcaster_storage_mngr * mcaster_storage_init();

// Retrieve current informations about some instance
mcaster_instance_record * mcaster_storage_get(mcaster_storage_mngr * msm, iid_t instance_number);

// Assign a client value to some instance
// The leader will try to deliver that value in that instance
// (unless forced by the protocol to do otherwise)
void mcaster_storage_assign_value(
    mcaster_storage_mngr * msm,
    mcaster_instance_record * mir, 
    void * cmd_value, 
    size_t cmd_size);

// In some occasions, the leader is forced to drop a client command
// in favor of one returned by Paxos phase 1
void mcaster_storage_replace_assigned_value(
    mcaster_instance_record * mir, 
    command_id * cmd_key, 
    void * cmd_value, 
    size_t cmd_size);
