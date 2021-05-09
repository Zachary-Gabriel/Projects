#include <string.h>
#include <assert.h>

#include "lp_utils.h"
#include "lp_clients_handling.h"
#include "lp_queue.h"
#include "lp_network.h"

struct clival_mngr_t {
    client_submit_cb cb;
    lpqueue * queue;
	udp_receiver * cli_recv_udp;
};

// CVM is a "singleton"
static clival_mngr * cvm = NULL;

static void client_values_mngr_handle_data (void * msg, size_t msg_size, lp_msg_type type) {
	if(type != client_submit) {
		LOG_MSG(WARNING, ("Warning, unknow message type %d\n", type));
		return;
	}
	cvm->cb(msg, msg_size);
	
	
	
}


bool client_values_mngr_UDP_init(
    client_submit_cb submit_cb /*Called when a client sends a value*/
    ) {

    // clival_mngr struct handles values sent by clients
    cvm = calloc(1, sizeof(clival_mngr));
    assert(cvm != NULL);

    //Save callback invoked on client submission
    cvm->cb = submit_cb;

	//Internal queue not initialized by default (TODO why?)
    cvm->queue = NULL;

	//Open listen socket for client commands, UDP
	cvm->cli_recv_udp = udp_receiver_init(
		lpconfig_get_ring_inbound_addr(),
		lpconfig_get_learners_inbound_port(), 
		client_values_mngr_handle_data);
	assert(cvm->cli_recv_udp != NULL);

    return true;
}

bool client_values_mngr_init_queue(unsigned queue_length) {
    if(cvm->queue != NULL) {
        printf("Queue is already initialized in for this values manager!\n");
        return NULL;
    }
    
    cvm->queue = queue_init(queue_length);
    if(cvm->queue == NULL) {
        printf("Failed to create queue!\n");
        return NULL;
    }

    return cvm;
}

bool cvm_get_next_value(void ** cmd_value_p, size_t * cmd_size_p) {
    assert(cvm->queue != NULL);
    return queue_pop(cvm->queue, cmd_value_p, cmd_size_p);
}

bool cvm_save_value(void * cmd_value, size_t cmd_size) {
    assert(cvm->queue != NULL);
    return queue_append(cvm->queue, cmd_value, cmd_size, true);

}
bool cvm_push_back_value(void * cmd_value, size_t cmd_size) {
    assert(cvm->queue != NULL);
    return queue_prepend(cvm->queue, cmd_value, cmd_size, false);
}

unsigned cvm_pending_list_size() {
	return queue_get_current_size(cvm->queue);
}

void cvm_print_bandwidth_stats(int time_now) {
	udp_receiver_print_stats(cvm->cli_recv_udp, time_now);
}
