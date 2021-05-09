#include "paxos_config.h"

// Interface for the stable storage required by the acceptors
// At the moment everything is actually performed in-memory

struct stable_storage_mngr_t;
typedef struct stable_storage_mngr_t stable_storage_mngr;

typedef struct instance_record_t {
    iid_t inst_number;
    ballot_t ballot;
    ballot_t accept_ballot;
    
    command_id accepted_cmd_key;
    size_t accepted_cmd_size;
    void * accepted_cmd;
    
    command_id proposed_cmd_key;
    size_t proposed_cmd_size;
    void * proposed_cmd;
    
}instance_record;

//Init storage manager
stable_storage_mngr * stable_storage_init();

//Get record based on instance number
instance_record *
ssm_get_record(
    stable_storage_mngr * ssm,
    iid_t inst_number);

//Force a certain record to be stable (i.e. synchronous write on disk)
void
ssm_update_record(
    stable_storage_mngr * ssm, 
    instance_record * ir);
//     
// void
// ssm_save_proposed_cmd(
//     stable_storage_mngr * ssm, 
//     instance_record * ir,
//     char* command_value, 
//     size_t command_size);

//Get the current value associated to an instance
void*    
ssm_get_command_slot(
    stable_storage_mngr * ssm,
    iid_t inst_number);

//Saves the final value chosen for this instance
void
ssm_save_delivered_value(
    stable_storage_mngr * ssm,
    iid_t inst_number,
    command_id * key);
