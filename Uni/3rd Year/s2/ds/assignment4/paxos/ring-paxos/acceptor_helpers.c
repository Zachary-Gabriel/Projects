static iid_t highest_instance_seen = 0;

struct acceptor_event_counters {
	long unsigned range_promise;
	long unsigned range_refuse;
	long unsigned p2_noval_refuse;
};
static struct acceptor_event_counters aec;

void acceptor_print_counters(void * arg) {
	UNUSED_ARG(arg);
	printf("**** Acceptor counters *** \n");
	
	PRINT_COUNT(aec.range_promise);
	PRINT_COUNT(aec.range_refuse);
	PRINT_COUNT(aec.p2_noval_refuse);
	
	PRINT_COUNT(highest_instance_seen);
	
	int time_now = time(NULL);
	printf("Ring BW:\n");
	udp_sender_print_stats(succ_send, time_now);
	udp_receiver_print_stats(pred_recv, time_now);
	
	printf("Mcast BW:\n");
	udp_receiver_print_stats(mcast_recv, time_now);
	printf("------------------------- \n\n");

}

void acceptor_flush_successor_socket() {
	udp_sender_force_flush(succ_send);
}

void acceptor_handle_phase1_msg(phase1_msg* msg, size_t size) {
    static char forward_buffer[MAX_MESSAGE_SIZE];
    
    assert(size == msg->cmd_size + sizeof(phase1_msg));
    
    //Retrieve information from stable storage, if any
    instance_record * ir = ssm_get_record(ssm, msg->inst_number);
    assert(ir->inst_number == msg->inst_number);
    
    //Already promised to a higher ballot
    if(msg->ballot < ir->ballot) {
        LOG_MSG(PAXOS, ("Refusing promise inst:%lu, ballot:%u (promised to bal:%u)\n",
            ir->inst_number, msg->ballot, ir->ballot));
		//The message is forwarded along the ring anyway, for two reasons:
		// I) Maybe a quorum is reached anyway without this acceptor
		// II) The highest ballot in the ring reaches the multicaster, who can restart
		//  phase 1 with a ballot high enough.
		if(ir->ballot > msg->highest_promised_ballot) {
			//Inform leader about highest ballot around
			msg->highest_promised_ballot = ir->ballot;
		}
		net_send_udp(succ_send, msg, size, phase1);
		return;
    }
    
    //The ballot is valid, make the promise
	if(msg->inst_number > highest_instance_seen) {
		//Track highest instance accepted/promised
		highest_instance_seen = msg->inst_number;
	}

    LOG_MSG(PAXOS, ("Promising inst:%lu, ballot:%u, prev_ballot:%u\n",
        ir->inst_number, msg->ballot, ir->ballot));
    // LOG_MSG_COND(PAXOS, (ir->ballot == 0), ("No previous ballot\n"));
    // LOG_MSG_COND(PAXOS, (ir->ballot != 0), ("Using previous ballot: %u\n", ir->ballot));

    ir->ballot = msg->ballot;
    ssm_update_record(ssm, ir);
    msg->promises_count += 1;
    
    //Create the message to be forwarded to successor
    
    //No command was accepted yet (or it's older than the one in the message)
    if(ir->accept_ballot <= msg->highest_accepted_ballot) {
        net_send_udp(succ_send, msg, size, phase1);
		return;
    }
    
    //This acceptor already accepted some command, and the corresponding ballot
    //  is greater than the one in message. Send the command and its ballot.
    LOG_MSG(PAXOS, ("Replacing highest accepted value in message, ballot:%u\n",
        ir->accept_ballot));
        
    phase1_msg * to_successor = (phase1_msg*)&forward_buffer;
    command_id * accepted_key = &ir->accepted_cmd_key;
    command_id * to_successor_key = &to_successor->cmd_key;
    
    memcpy(to_successor, msg, sizeof(msg));
    to_successor->highest_accepted_ballot = ir->accept_ballot;
    CMD_KEY_COPY(to_successor_key, accepted_key);
    to_successor->cmd_size = ir->accepted_cmd_size;
    memcpy(to_successor->cmd_value, ir->accepted_cmd, ir->accepted_cmd_size);
    net_send_udp(succ_send, to_successor, PH1_MSG_SIZE(to_successor), phase1);
    
};

void acceptor_handle_phase1_range_msg(phase1_range_msg* msg, size_t size) {
	assert(size == sizeof(phase1_range_msg));
	
	
	//Instances for which leader is asking for promise were never
	// executed so far, we can safely grant the request for all of them
	if(msg->from > highest_instance_seen) {
				
		instance_record * ir;
		iid_t i;
		
		LOG_MSG(PAXOS, ("Promising for P1 instances range [%lu...%lu]\n", 
			msg->from, msg->to));
		COUNT_EVENT(PAXOS, aec.range_promise);
		
		highest_instance_seen = msg->to;

		//Store promised ballot for each instance in range
		for(i = msg->from; i <= msg->to; i++) {
			ir = ssm_get_record(ssm, i);
		    assert(ir->inst_number == i);
			assert(ir->ballot == 0);
			
			ir->ballot = msg->ballot;
			ssm_update_record(ssm, ir);
		}

		//Forward the message to successor
		msg->promises_count += 1;
		net_send_udp(succ_send, msg, sizeof(phase1_range_msg), phase1_range);
		
	} else {
		LOG_MSG(PAXOS, ("Breaking down instance range [%lu...%lu]\n", 
			msg->from, msg->to));
		COUNT_EVENT(PAXOS, aec.range_refuse);

		//Some instances in the requested range are already 'dirty'
		// (i.e. phase 1 or 2 was executed).
		//Transform into normal phase1 messages and handle individually.
		// TODO ugly hack!
		static phase1_msg p1_temp_msg;
		iid_t i;
		for(i = msg->from; i <= msg->to; i++) {
			p1_temp_msg.inst_number = i;
			p1_temp_msg.ballot = msg->ballot;
			p1_temp_msg.promises_count = msg->promises_count;
			p1_temp_msg.highest_accepted_ballot = 0;
			p1_temp_msg.highest_promised_ballot = 0;
			CMD_KEY_CLEAR((&p1_temp_msg.cmd_key));
			p1_temp_msg.cmd_size = 0;
			acceptor_handle_phase1_msg(&p1_temp_msg, PH1_MSG_SIZE_S(p1_temp_msg));
		}
	}	
}


void acceptor_handle_phase2_msg(phase2_msg* msg, size_t size) {
    assert(size == sizeof(phase2_msg));
    
    //Retrieve information from stable storage, if any
    instance_record * ir = ssm_get_record(ssm, msg->inst_number);
    assert(ir->inst_number == msg->inst_number);
    
    //Ballot is less than promised ballot
    if(msg->ballot < ir->ballot) {
        LOG_MSG(PAXOS, ("Refusing to accept, inst:%lu, bal:%u promised to bal:%u\n", 
            msg->inst_number, msg->ballot, ir->ballot));
        return;
    }
       
    command_id * key = &msg->cmd_key;
    command_id * accepted_key = &ir->accepted_cmd_key;
    command_id * proposed_key = &ir->proposed_cmd_key;
    
    bool is_proposed_cmd = CMD_KEY_EQUALS(key, proposed_key);
    bool is_accepted_cmd = CMD_KEY_EQUALS(key, accepted_key);
    
    // It should never be the case that a command is both stored in 'proposed' and 'accepted'
	LOG_MSG_COND(DEBUG, (is_proposed_cmd && is_accepted_cmd), 
		("Command %s for instance %lu is stored as both proposed and accepted\n", 
		print_cmd_key(key), msg->inst_number));
    assert((is_proposed_cmd && is_accepted_cmd) == false);
    
    //Command for this identifier is not known, cannot accept it
    // [map-accept-graph Edge A1, A7, A3]
    if((!is_accepted_cmd) && (!is_proposed_cmd)) {
        LOG_MSG(PAXOS, ("Refusing to accept, inst:%lu, unknown command value\n", 
            msg->inst_number));
		COUNT_EVENT(PAXOS, aec.p2_noval_refuse);
        return;        
    }
    
    //Ballot is valid, the command is going to be accepted
	if(msg->inst_number > highest_instance_seen) {
		//Track highest instance accepted/promised
		highest_instance_seen = msg->inst_number;
	}
	
    void * command_slot = command_slot = ssm_get_command_slot(ssm, msg->inst_number);

    //This command is already accepted, accept it again
    // [map-accept-graph Edge A4]
    if(is_accepted_cmd) {
        LOG_MSG(PAXOS, ("Accept inst:%lu, previously accepted value\n",
            msg->inst_number));
        
        //Mapping for another command is stored in 'proposed', 
        // can now get rid of it
        // [map-accept-graph Edge A6]
        if(ir->proposed_cmd != NULL) {
            assert(CMD_KEY_EQUALS(key, proposed_key) == false);
            assert(ir->proposed_cmd != command_slot);
            
            clear_cmd_key(proposed_key);
            free(ir->proposed_cmd);
            ir->proposed_cmd = NULL;
            ir->proposed_cmd_size = 0;
        }        
        goto acceptor_p2_store_and_send;
    }
    
    //The mapping is known, no command was accepted yet in this instance
    //In this case the proposed command was saved already in the accept slot, 
    // there's no need to copy it there
    // [map-accept-graph Edge A2]
    if(is_proposed_cmd && ir->accepted_cmd == NULL) {
        LOG_MSG(PAXOS, ("Accept inst:%lu, map known, no previous accept\n",
            msg->inst_number));
        assert(ir->proposed_cmd == command_slot);

        CMD_KEY_COPY(accepted_key, proposed_key);
        ir->accepted_cmd = ir->proposed_cmd;
        ir->accepted_cmd_size = ir->proposed_cmd_size;
		CMD_KEY_CLEAR(proposed_key);
        ir->proposed_cmd = NULL;
        ir->proposed_cmd_size = 0;
        goto acceptor_p2_store_and_send;
    }
    
    //There is a stored mapping AND some previously accepted value
    //Accept the new value and overwrite the old one
    // [map-accept-graph Edge A8]
    LOG_MSG(PAXOS, ("Accept inst:%lu, map known, replacing previous accept\n",
        msg->inst_number));
    assert(ir->accepted_cmd != NULL && ir->proposed_cmd != NULL &&
        ir->accepted_cmd != ir->proposed_cmd);
    assert(is_proposed_cmd);
    
    memcpy(command_slot, ir->proposed_cmd, ir->proposed_cmd_size);
    ir->accepted_cmd_size = ir->proposed_cmd_size;
    CMD_KEY_COPY(accepted_key, proposed_key);

    ir->proposed_cmd_size = 0;
    free(ir->proposed_cmd);
    clear_cmd_key(proposed_key);
    goto acceptor_p2_store_and_send;


// This section is executed by all cases in which the acceptor does accepts
acceptor_p2_store_and_send:

    //Save ballot just accepted
    ir->ballot = msg->ballot;
    ir->accept_ballot = msg->ballot;

    //Save changes in stable storage
    ssm_update_record(ssm, ir);
    
    //Forward message to successor
    msg->accepts_count += 1;
    net_send_udp(succ_send, msg, sizeof(phase2_msg), phase2);
};

void acceptor_handle_cmdmap_msg(cmdmap_msg* msg, size_t size) {
    assert(size == CMDMAP_MSG_SIZE(msg));

    // Each cmdmap contains a key and a command, 
    // consensus is executed on the key, while the command is delivered to learners
    // Acceptors store this mapping (which is sent by the multicaster and 
    // already contains the instance id in which the value is proposed)

    //Retrieve information from stable storage, if any
    instance_record * ir = ssm_get_record(ssm, msg->inst_number);
    assert(ir->inst_number == msg->inst_number);
    
    command_id * key = &msg->cmd_key;
    command_id * proposed_key = &ir->proposed_cmd_key;
    command_id * accepted_key = &ir->accepted_cmd_key;
    void * command_slot = command_slot = ssm_get_command_slot(ssm, msg->inst_number);
    
    //This mapping is known because it was accepted
    //[map-accept-graph Edge M4]
    if(ir->accepted_cmd != NULL && CMD_KEY_EQUALS(key, accepted_key)) {
        assert(ir->accepted_cmd_size == msg->cmd_size);
        assert(strncmp(ir->accepted_cmd, msg->cmd_value, msg->cmd_size) == 0);
        LOG_MSG(PAXOS, ("Discarding map, already accepted\n"));

        //We stored another mapping for this instance, since we only keep
        // the most recent one, get rid of it
        //[map-accept-graph Edge M6]
        if(ir->proposed_cmd != NULL) {
            assert(ir->proposed_cmd != ir->accepted_cmd);
            LOG_MSG(PAXOS, ("Freeing malloccated map\n"));

            free(ir->proposed_cmd);
            ir->proposed_cmd = NULL;
            ir->proposed_cmd_size = 0;
            clear_cmd_key(proposed_key);
        }
        return;
    }
    
    //This mapping is already stored
    //[map-accept-graph Edge M3, M7]
    if(ir->proposed_cmd != NULL && CMD_KEY_EQUALS(key, proposed_key)) {
        assert(ir->proposed_cmd_size == msg->cmd_size);
        assert(strncmp(ir->proposed_cmd, msg->cmd_value, msg->cmd_size) == 0);
        LOG_MSG(PAXOS_DBG, ("Discarding map inst:%lu, already known\n", 
            msg->inst_number));
            
        return;
    }
    
    //No value was accepted yet for this instance, 
    //Store the mapping directly in the 'acceptance' slot.
    //(if it's later accepted, there is no need to copy!)
    //[map-accept-graph Edge M1, M2]
    if(ir->accepted_cmd == NULL) {
        assert(ir->accepted_cmd_size == 0);
        LOG_MSG(PAXOS, ("Saving map inst:%lu\n", msg->inst_number));

        ir->proposed_cmd = command_slot;
        ir->proposed_cmd_size = msg->cmd_size;
        memcpy(command_slot, msg->cmd_value, msg->cmd_size);
        memcpy(proposed_key, key, sizeof(command_id));
        return;
    }
    
    //Some value was accepted for this instance (different from the received mapping)
    //Even if we already accepted something, we received the mapping for a different command
    //Hopefully this case is very rare (i.e. Leader failure)
    
    //We store it in a different area (malloc rather than instance slot)
    //[map-accept-graph Edge M5]
    if(ir->proposed_cmd == NULL) {
        LOG_MSG(PAXOS, ("Saving secondary map inst:%lu\n", msg->inst_number));
        
        ir->proposed_cmd = malloc(msg->cmd_size);
        ir->proposed_cmd_size = msg->cmd_size;
        memcpy(ir->proposed_cmd, msg->cmd_value, msg->cmd_size);
        CMD_KEY_COPY(&ir->proposed_cmd_key, &msg->cmd_key);
        return;
    }
    
    //Same as previous case but there's already some other mapping stored there, 
    // Replace it (always keep the most recent only)
    //[map-accept-graph Edge M8]
    assert(((ir->proposed_cmd_size == msg->cmd_size) && 
        (strncmp(ir->proposed_cmd, msg->cmd_value, msg->cmd_size) == 0)) == false);
    LOG_MSG(PAXOS, ("Replacing secondary map inst:%lu\n", msg->inst_number));
      
    free(ir->proposed_cmd);
    ir->proposed_cmd = malloc(msg->cmd_size);
    memcpy(ir->proposed_cmd, msg->cmd_value, msg->cmd_size);
    ir->proposed_cmd_size = msg->cmd_size;
    CMD_KEY_COPY(&ir->proposed_cmd_key, &msg->cmd_key);
    
};

void 
acceptor_handle_acceptance_msg(acceptance_msg* msg, size_t size) {
    assert(size == sizeof(acceptance_msg));

    //Multicaster is telling us that a value was chosen (consensus) in some instance
    //(This is not relevant unless the acceptor is also a learner)
    LOG_MSG(PAXOS, ("Final value for inst:%lu received\n", msg->inst_number));
    
    ssm_save_delivered_value(ssm, msg->inst_number, &msg->cmd_key);
}

