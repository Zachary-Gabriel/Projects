#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <sys/time.h>

#include "paxos_config.h"
#include "lp_delivery_queue.h"
#include "lp_timers.h"
#include "lp_utils.h"
#include "lp_config_parser.h"

static periodic_event * periodic_gap_check;

struct timeval request_timeout = {0, 10000}; //TODO make this a config parameter

typedef struct cmd_slot_t {
    size_t size;
    char data[MAX_MESSAGE_SIZE];
} cmd_slot;

typedef struct dq_entry_t {
    iid_t inst_number;
    command_id cmd_key;
    bool has_mapping;
    bool has_final_value;
	struct timeval cmdmap_request_timeout;
	struct timeval finval_request_timeout;
} dq_entry;

struct delivery_queue_t {
    size_t queue_size;
    dq_entry * queue_array;
    cmd_slot * cmd_slot_array;

    iid_t highest_delivered;
    iid_t highest_seen_closed;
    iid_t highest_seen_cmdmap;
        
    deliver_callback del_cb;
    missing_cmdmap_callback mcm_cb;
    missing_acceptance_callback mac_cb;
    post_check_callback pc_cb;
    
};

static
void dq_clear_entry(dq_entry * e) {
    e->inst_number = 0;
    command_id * key = &e->cmd_key;
    CMD_KEY_CLEAR(key);
    e->has_mapping = false;
    e->has_final_value = false;
	e->cmdmap_request_timeout.tv_sec = 0;
	e->cmdmap_request_timeout.tv_usec = 0;
	e->finval_request_timeout.tv_sec = 0;
	e->finval_request_timeout.tv_usec = 0;
}

static
dq_entry * dq_get_entry(delivery_queue * dq, iid_t inst_number) {
	assert(inst_number > 0);
	assert(inst_number <= dq->highest_delivered + dq->queue_size);
	
	dq_entry * e = &dq->queue_array[inst_number % dq->queue_size];
	
	LOG_MSG_COND(DEBUG, (e->inst_number != inst_number && e->inst_number != 0),
		("Delivery queue entry is numbered:%lu, expected:%lu\n", e->inst_number, inst_number));
	assert(e->inst_number == inst_number || e->inst_number == 0);
	e->inst_number = inst_number;
    return e;
}

static
cmd_slot * dq_get_slot(delivery_queue * dq, dq_entry * e) {
	
	cmd_slot * cs = &dq->cmd_slot_array[e->inst_number % dq->queue_size];
    
    return cs;
}

static
void dq_periodic_check(void * arg) {
    delivery_queue * dq = arg;
    dq_entry * e;

// TODO try deliver here?

	struct timeval time_now;
	gettimeofday(&time_now, NULL);

    iid_t i;
    iid_t upper_limit = IID_MAX(dq->highest_seen_closed, dq->highest_seen_cmdmap);
    for(i = dq->highest_delivered+1; i <= upper_limit; i++) {

		//Dont try to read beyond the limits of the current circular buffer
		if(i >= (dq->highest_delivered + dq->queue_size)) {
			break;
		}
        
        //Get entry in circular buffer
        e = dq_get_entry(dq, i);
		assert(e->inst_number == i);

		//This instance is deliverable as it is, skip to next
		if(e->has_mapping && e->has_final_value) {
			continue;
		}

        //Unknown mapping, request it
        if( ! e->has_mapping ) {
	
			//Not already requested recently
			if((e->cmdmap_request_timeout.tv_sec == 0 &&
			   e->cmdmap_request_timeout.tv_usec == 0) ||
			   timer_is_expired(&e->cmdmap_request_timeout, &time_now)) {
				LOG_MSG(DELIVERY_Q, ("Missing map for inst:%lu\n", e->inst_number));
				dq->mcm_cb(i);
				timer_set_timeout(&time_now, &e->cmdmap_request_timeout, &request_timeout);
			}
        }
        
        //Some instance higher than this one is already closed
        //request final value for this one.
        if(i < dq->highest_seen_closed) {
	
			//Not already requested recently
			if((e->finval_request_timeout.tv_sec != 0 &&
			   e->finval_request_timeout.tv_usec != 0) ||
			   timer_is_expired(&e->finval_request_timeout, &time_now)) {
				LOG_MSG(DELIVERY_Q, ("Missing acceptance for inst:%lu\n", e->inst_number));
				dq->mac_cb(i);
				timer_set_timeout(&time_now, &e->finval_request_timeout, &request_timeout);
			}
		}        
    }
    //Invoke post-check callback if set
    if(dq->pc_cb != NULL) {
        dq->pc_cb();
    }
}

delivery_queue * delivery_queue_init(
    deliver_callback del_cb,
    missing_cmdmap_callback mcm_cb,
    missing_acceptance_callback mac_cb,
    post_check_callback pc_cb) 
{
    
    delivery_queue * dq = calloc(1, sizeof(delivery_queue));
    assert(dq != NULL);
    
    dq->queue_size = (size_t)lpconfig_get_working_set_size();
    dq->highest_delivered = 0;
	dq->highest_seen_closed = 0;
	dq->highest_seen_cmdmap = 0;
    
    dq->del_cb = del_cb;
    dq->mcm_cb = mcm_cb;
    dq->mac_cb = mac_cb;
    dq->pc_cb = pc_cb;
    
    dq->cmd_slot_array = calloc(dq->queue_size, sizeof(cmd_slot));
    assert(dq->cmd_slot_array != NULL);

    dq->queue_array = calloc(dq->queue_size, sizeof(dq_entry));
    assert(dq->queue_array != NULL);
    
    dq_entry * e;
    iid_t i;
    for(i = 0; i < dq->queue_size; i++) {
        e = &dq->queue_array[i];
        dq_clear_entry(e);
    }
    
    //Set periodic event for detecting gaps
    periodic_gap_check = set_periodic_event(lpconfig_get_delivery_check_interval(), dq_periodic_check, dq);
    
    return dq;
    
}

void dq_deliver_loop(delivery_queue * dq) {
	dq_entry * e;
	cmd_slot * cs;
	
	e = dq_get_entry(dq, dq->highest_delivered+1);
	
	//Next undelivered can now be delivered
	while(e->has_mapping && e->has_final_value) {
		LOG_MSG(DELIVERY_Q, ("Delivering inst:%lu\n", dq->highest_delivered+1));
		cs = dq_get_slot(dq, e);
		//Invoke learner callback (a-deliver)
		dq->del_cb(&cs->data, cs->size);
		//Clear instance, we don't need it anymore
		dq_clear_entry(e);

		// move cursor of next deliverable
		dq->highest_delivered += 1;
		
		e = dq_get_entry(dq, dq->highest_delivered+1);
	}
}

void delivery_queue_handle_command_map(
    delivery_queue * dq, 
    iid_t inst_number,
    command_id * cmd_key,
    size_t cmd_size,
    void * cmd_value
    )
{
    //This is too far in the future and would override 
    // some other instance, ignore it
    if(inst_number >= (dq->highest_delivered + dq->queue_size)) {
        LOG_MSG(DELIVERY_Q, ("Ignoring future instance %lu\n", inst_number));
        return;
	} else if (inst_number <= dq->highest_delivered) {
        LOG_MSG(DELIVERY_Q, ("Ignoring old instance %lu\n", inst_number));
        return;
	}
	
	//Inst number is within working bounds
    
    //Keep track of highest seen mapping
    if(inst_number > dq->highest_seen_cmdmap) {
        dq->highest_seen_cmdmap = inst_number;
    }
    
    dq_entry * e = dq_get_entry(dq, inst_number);
	assert(e->inst_number == inst_number);

	cmd_slot * cs = dq_get_slot(dq, e);
	assert(cs != NULL);
	
	//We already know all about this instance 
	// (but it wasn't delivered yet, probably because of some gap)
	if(e->has_final_value && e->has_mapping) {
		LOG_MSG(DELIVERY_Q, ("Instance %lu is already deliverable\n", inst_number));
		return;
	}
	
	//We know some mapping already but not the chosen value key
	if(e->has_mapping) {
		// By default, keep the last mapping received

		//Overwrite only if different [edge M2]
		if(CMD_KEY_EQUALS(cmd_key, (&e->cmd_key))) {
			assert(cs->size == cmd_size);
			assert(memcmp(cmd_value, cs->data, cmd_size) == 0);
		} else {
			LOG_MSG(DELIVERY_Q, ("Replace mapping of inst:%lu, %s ->", 
				inst_number, print_cmd_key(&e->cmd_key))); 
			LOG_MSG(DELIVERY_Q, (" %s\n", print_cmd_key(cmd_key)));
			
			CMD_KEY_COPY(&e->cmd_key, cmd_key);
			cs->size = cmd_size;
			memcpy(cs->data, cmd_value, cmd_size);
		}
		return;
	}
	
	//We know the key of the value chosen, but not the command value mapping
	if(e->has_final_value) {
		//We received the mapping that we were waiting for 
		if(CMD_KEY_EQUALS(cmd_key, (&e->cmd_key))) {
			// Mapping matches deliverable key
			// Save the value and mark as deliverable [edge M4]
			LOG_MSG(DELIVERY_Q, ("Got mapping for a known chosen value, inst:%lu\n",
				inst_number));
			cs->size = cmd_size;
			memcpy(cs->data, cmd_value, cmd_size);
			e->has_mapping = true;
			//It may be possible to deliver this (and following) values now
			dq_deliver_loop(dq);
		} else {
			//Mapping does not match, since accepted value won't change
			// we can safely drop it [edge M3]
			LOG_MSG(DELIVERY_Q, ("Got mapping different from chosen value, inst:%lu\n",
				inst_number));
		}
		return;
	}
	
	//Mapping AND chosen key are not known, save the mapping received
	LOG_MSG(DELIVERY_Q, ("Saving mapping of new inst:%lu, %s\n",
		e->inst_number, print_cmd_key(cmd_key)));
	CMD_KEY_COPY((&e->cmd_key), cmd_key);
	cs->size = cmd_size;
	memcpy(cs->data, cmd_value, cmd_size);
	e->has_mapping = true;			
	
}

void delivery_queue_handle_acceptance(
    delivery_queue * dq, 
    iid_t inst_number,
    command_id * cmd_key
    )
{
	
	//Keep track of highest seen mapping
    if(inst_number > dq->highest_seen_closed) {
        dq->highest_seen_closed = inst_number;
    }

    //This is too far in the future and would override 
    // some other instance, ignore it
    if(inst_number >= (dq->highest_delivered + dq->queue_size)) {
        LOG_MSG(DELIVERY_Q, ("Ignoring future instance %lu\n", inst_number));
        return;
	} else if (inst_number <= dq->highest_delivered) {
        LOG_MSG(DELIVERY_Q, ("Ignoring old instance %lu\n", inst_number));
        return;
	}
	
	//Inst number is within working bounds

    dq_entry * e = dq_get_entry(dq, inst_number);
	bool same_key = CMD_KEY_EQUALS((&e->cmd_key), cmd_key);
	
	//We already know all about this instance 
	// (but it wasn't delivered yet, probably because of some gap)
	if(e->has_final_value && e->has_mapping) {
		assert(same_key == true);
		return;
	}
	
	//We know some mapping already but not the chosen value key
	if(e->has_mapping) {
		if(same_key) { 
			//The key of the mapping that we have has been chosen, 
			// this value can be delivered [edge A4]
			LOG_MSG(DELIVERY_Q, ("Received final value for known mapping, inst:%lu is deliverable\n",
				inst_number));
			e->has_final_value = true;
			//It may be possible to deliver this (and following) values now
			dq_deliver_loop(dq);
		} else {
			//The key of the final value is different from the mapping that we have
			//Save the final key (that won't change), drop the mapping [edge A3]
			//TODO request this mapping again!
			LOG_MSG(DELIVERY_Q, ("Received final value inst:%lu discarding different mapping!\n",
				inst_number));
			e->has_mapping = false;
			CMD_KEY_COPY(&e->cmd_key, cmd_key);
		}
		return;
	}
	
	//We know the key of the value chosen, but not the command value mapping [edge A2]
	if(e->has_final_value) {
		assert(CMD_KEY_EQUALS((&e->cmd_key), cmd_key));		
		return;
	}
	
	//Nothing is known, store the final value key [edge A1]
	CMD_KEY_COPY(&e->cmd_key, cmd_key);
	e->has_final_value = true;
	LOG_MSG(DELIVERY_Q, ("Learned final value inst:%lu, mapping is not known\n",
		inst_number));
}
