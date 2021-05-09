#include <stdlib.h>
#include <assert.h>

#include "lp_utils.h"
#include "lp_stable_storage.h"
#include "lp_config_parser.h"

typedef struct command_slot_t {
    char data[MAX_MESSAGE_SIZE];
} command_slot;

struct stable_storage_mngr_t {
    unsigned size;
    instance_record * instances_array;
    command_slot * commands_array;
};

static void clear_instance_record(instance_record * ir) {
    ir->inst_number = 0;
    ir->ballot = 0;
    ir->accept_ballot = 0;

    clear_cmd_key(&ir->accepted_cmd_key);
    ir->accepted_cmd_size = 0;
    ir->accepted_cmd = NULL;
    
    clear_cmd_key(&ir->proposed_cmd_key);
    ir->proposed_cmd_size = 0;
    ir->proposed_cmd = NULL;
}

stable_storage_mngr * stable_storage_init() {
    
    stable_storage_mngr * ssm = calloc(1, sizeof(stable_storage_mngr));
    assert(ssm != NULL);
    ssm->size = (unsigned)lpconfig_get_working_set_size();

    ssm->commands_array = calloc(ssm->size, sizeof(command_slot));
    assert(ssm->commands_array != NULL);
        
    ssm->instances_array = calloc(ssm->size, sizeof(instance_record));
    assert(ssm->instances_array != NULL);
    unsigned int i;
    for(i = 0; i < ssm->size; i++) {
        clear_instance_record(&ssm->instances_array[i]);
    }
    
    return ssm;
}

static 
instance_record *
ssm_get_archived_record(
    stable_storage_mngr * ssm,
    iid_t inst_number) 
{
	// LOG_MSG(ACC_STORE, ("Retrieving inst:%lu from archive\n", inst_number));
    //TODO low_priority implement
    UNUSED_ARG(ssm);
    UNUSED_ARG(inst_number);
	printf("Acceptor archive does not exist! Cannot retrieve inst:%lu\n", inst_number);
	assert(1 == 0);
    return NULL;
}

static 
void
ssm_archive_record(
    stable_storage_mngr * ssm,
    instance_record * ir) 
{
	LOG_MSG(ACC_STORE, ("Archiving inst:%lu\n", ir->inst_number));
    //TODO low_priority implement
    UNUSED_ARG(ssm);
    UNUSED_ARG(ir);
}

instance_record *
ssm_get_record(
    stable_storage_mngr * ssm,
    iid_t inst_number) 
{
	
	//Get record pointer in circular buffer
    instance_record * ir;
    ir = &ssm->instances_array[inst_number % ssm->size];

	if(ir->inst_number == 0) {
		//This record slot is unused, initialize it
		ir->inst_number = inst_number;
	} else if(ir->inst_number < inst_number) {
		//Slot contains old instance, we need to reuse it for a new one.
		ssm_archive_record(ssm, ir);
		clear_instance_record(ir);
		ir->inst_number = inst_number;
	} else if(ir->inst_number > inst_number) {
		//Requested instance was overwritten by newer instance
		ir = ssm_get_archived_record(ssm, inst_number);
	} else {
		//This record contains the requested instance info
		//ir->inst_number == inst_number
	}

	return ir;
}

void
ssm_update_record(
    stable_storage_mngr * ssm, 
    instance_record * ir) {
        // TODO implement
        // TODO this is still missing in different places
        UNUSED_ARG(ssm);
        UNUSED_ARG(ir);
    }
    
void *
ssm_get_command_slot(
    stable_storage_mngr * ssm,
    iid_t inst_number) {

        instance_record * ir = ssm_get_record(ssm, inst_number);
        assert(ir->inst_number == inst_number);
        void * slot = &ssm->commands_array[inst_number % ssm->size];
        return slot;
    }

void
ssm_save_delivered_value(
    stable_storage_mngr * ssm,
    iid_t inst_number,
    command_id * key) {

        // TODO implement
        UNUSED_ARG(ssm);
        UNUSED_ARG(inst_number);
        UNUSED_ARG(key);
    }
