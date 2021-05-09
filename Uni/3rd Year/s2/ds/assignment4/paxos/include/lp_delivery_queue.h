#include <stdlib.h>

#include "paxos_config.h"

//The delivery queue is used in the learners to buffer values
// and make sure they are delivered in order to the application on top

struct delivery_queue_t;
typedef struct delivery_queue_t delivery_queue;

//Prototypes to allow customization of delivery queue:

//Action to take when the next value is being delivered
typedef void(*deliver_callback)(void*, size_t);
//Action to take when the value for the next command to deliver is unknown
typedef void(*missing_cmdmap_callback)(iid_t);
//Action to take when the next command to deliver is unknown, but some future
// command is already known (a "gap" is detected)
typedef void(*missing_acceptance_callback)(iid_t);
//Action to take after checking for gaps
typedef void(*post_check_callback)(void);

//Initialization of delivery queue
delivery_queue * delivery_queue_init(
    deliver_callback del_cb,
    missing_cmdmap_callback mcm_cb,
    missing_acceptance_callback mac_cb,
    post_check_callback pc_cb);

//Action triggered when a command_map message is received
void delivery_queue_handle_command_map(
    delivery_queue * dq, 
    iid_t inst_number,
    command_id * cmd_key,
    size_t cmd_size,
    void * cmd_value
    );

//Action triggered when an acceptance message is received
void delivery_queue_handle_acceptance(
    delivery_queue * dq, 
    iid_t inst_number,
    command_id * cmd_key
    );

//Action periodically triggered to deliver chosen values to the application
void dq_deliver_loop(delivery_queue * dq);
