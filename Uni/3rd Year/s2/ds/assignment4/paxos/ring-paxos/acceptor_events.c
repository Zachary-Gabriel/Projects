void on_predecessor_msg(void* msg, size_t size, lp_msg_type type) {
    if(am_i_leader()) {
        //This process is currently leader/multicaster
        switch(type) {
            case phase1:
                mcaster_handle_phase1_msg((phase1_msg*)msg, size);
                break;
			case phase1_range:
				mcaster_handle_phase1_range_msg((phase1_range_msg*)msg, size);
				break;
            case phase2:
                mcaster_handle_phase2_msg((phase2_msg*)msg, size);
                break;
			case map_request:
				mcaster_handle_map_request((map_requests_msg*)msg, size);
				break;
			case chosenval_request:
				mcaster_handle_chosenval_request((chosencmd_requests_msg*)msg, size);
				break;
            default:
                LOG_MSG(WARNING, ("Warning: received message of unknown type %d\n", (int)type))
        }
    } else {
        //This process is currently NOT leader/multicaster
        switch(type) {
            case phase1:
                acceptor_handle_phase1_msg((phase1_msg*)msg, size);
                break;
			case phase1_range:
				acceptor_handle_phase1_range_msg((phase1_range_msg*)msg, size);
				break;
            case phase2:
                acceptor_handle_phase2_msg((phase2_msg*)msg, size);
                break;
            default:
                LOG_MSG(WARNING, ("Warning: received message of unknown type %d\n", (int)type))
        }
    }
}

void on_multicast_msg(void* msg, size_t size, lp_msg_type type) {    
    switch(type) {
        case command_map:
            acceptor_handle_cmdmap_msg((cmdmap_msg*)msg, size);
            break;
		case acceptance:
			acceptor_handle_acceptance_msg((acceptance_msg*)msg, size);
			break;
		case phase2:
			//First phase2a is sent through multicast, only the first acceptor in the ring
			// should process it, then it's going to be forwarded along the UDP ring
			if(am_i_first_in_ring()) {
				acceptor_handle_phase2_msg((phase2_msg*)msg, size);				
			}
			break;
        default:
            LOG_MSG(WARNING, ("Warning: received message of unknown type %d\n", (int)type))
    }
}

void on_repeat_request(void* msg, size_t size) {
    // TODO implement
    UNUSED_ARG(msg);
    UNUSED_ARG(size);
}


// Event handler for configuration change
void on_configuration_change(config_mngr * old, config_mngr * new) {
    LOG_MSG(INFO, ("Configuration changed!\n"));
    // TODO low_priority
    UNUSED_ARG(old);
    UNUSED_ARG(new);
}

// Event handler for topology change
// (i.e. Some node crashes and there's a new ring overlay)
void on_topology_change() {
    LOG_MSG(INFO, ("Topology changed!\n"));
    
    // TODO low_priority
    //Recreate sockets if needed
    // Switch to leader/coordinator/multicaster 
    // mode if required.
}
