#include <string.h>
#include <assert.h>

#include "lp_mcaster_storage.h"
#include "lp_utils.h"

struct mcaster_storage_mngr_t {
    uint16_t seq_number;
    long unsigned array_size;
    mcaster_instance_record instances_array[0];
};


static void create_value_key(mcaster_storage_mngr * msm, command_id * cmd_key) {
    cmd_key->mcaster_id = lpconfig_get_self_acceptor_id();
    cmd_key->mcaster_incarnation = lpconfig_get_incarnation_number();
    cmd_key->cmd_seqnum = msm->seq_number;
    msm->seq_number += 1;
    if(msm->seq_number == 0xFFFF) {
        msm->seq_number = 1; 
    }
}

static void
mcaster_storage_clear_record(mcaster_instance_record * mir) {
	mir->inst_number = 0;
    mir->status = empty;
    mir->ballot = 0;
    mir->assigned_cmd_value = NULL;
    mir->assigned_cmd_size = 0;
    CMD_KEY_CLEAR((&mir->assigned_cmd_key));
}

mcaster_storage_mngr * mcaster_storage_init() {
	
	int number_of_entries = lpconfig_get_working_set_size()*2;
    
    size_t total_size = sizeof(mcaster_storage_mngr) + 
        (sizeof(mcaster_instance_record) * number_of_entries);
        
    mcaster_storage_mngr * msm = calloc(1, total_size);
    assert(msm != NULL);

	int i = 0;
	mcaster_instance_record * mir;
	for(i = 0; i < (number_of_entries); i++) {
		mir = &msm->instances_array[i];
		mcaster_storage_clear_record(mir);
	}
    
    msm->seq_number = 1;
    msm->array_size = number_of_entries;
    
    return msm;
}

mcaster_instance_record * mcaster_storage_get(mcaster_storage_mngr * msm, iid_t inst_number) {
    mcaster_instance_record * mir;
	LOG_MSG(MCAST_STORE, ("Retrieving record %ld\n", inst_number));
    
    mir = &msm->instances_array[inst_number % msm->array_size];
    
    //This instance record slot was already used for this instance
    if(mir->inst_number == 0) {
		//Empty record, initialize it
		mir->inst_number = inst_number;
        mir->status = ready;
	} else if (mir->inst_number < inst_number) {
		//Record still contains old instance, archive old, clear, initialize new
		assert(mir->status == done);
		free(mir->assigned_cmd_value);
		mcaster_storage_clear_record(mir);
		mir->inst_number = inst_number;
        mir->status = ready;
	} else if (mir->inst_number > inst_number) {
		//Record contains newer instance, 
		// old information must be retrieved in some other way
		//TODO re-execution of phase 1 here...
		LOG_MSG(WARNING, ("Old info for inst:%lu was already overwritten!\n", inst_number));
		assert(1 == 0);
    } else { 
		//mir->inst_number == instance_number
		//Record corresponds to request
	}
    return mir;
}

void mcaster_storage_assign_value(
    mcaster_storage_mngr * msm,
    mcaster_instance_record * mir, 
    void * cmd_value, 
    size_t cmd_size) 
{

    assert(mir->assigned_cmd_value == NULL);
    
    //Save the value 
    mir->assigned_cmd_value = cmd_value;
    mir->assigned_cmd_size = cmd_size;
    command_id * cmd_key = &mir->assigned_cmd_key;

    //Create a unique key for indirect consensus
    create_value_key(msm, cmd_key);
}

void mcaster_storage_replace_assigned_value(
    mcaster_instance_record * mir, 
    command_id * cmd_key, 
    void * cmd_value, 
    size_t cmd_size) 
{
    //Getting here means that phase 1 returned some value different
    // from the one we are proposing.
    LOG_MSG(MCAST_STORE, ("Replacing assigned command for inst:%lu\n", mir->inst_number));

    //Replace the 'assigned' key with the one found
    command_id * assigned_cmd_key = &mir->assigned_cmd_key;
    CMD_KEY_COPY(assigned_cmd_key, cmd_key);
    

    //If we get lucky, the identifier is different but the value is the same
    if(mir->assigned_cmd_size == cmd_size &&
    memcmp(mir->assigned_cmd_value, cmd_value, cmd_size) == 0) {
        //Just the key must be replaced
        return;
    }
    
    
    //Push back our command in the queue, it will be sent later 
    // in some other instance. Then do phase 2 with the returned one.
    bool success;
    success = cvm_push_back_value(mir->assigned_cmd_value, mir->assigned_cmd_size);
    assert(success == true);

    //Copy the received value there;
    mir->assigned_cmd_size = cmd_size;
    mir->assigned_cmd_value = malloc(cmd_size);
    memcpy(mir->assigned_cmd_value, cmd_value, cmd_size);
}
