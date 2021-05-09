#include "lp_delivery_queue.h"

//A custom initialization for the learner, useful for adding
// events to the libevent loop
typedef void(*custom_init_func)(void);

// Initializes a learner in the current process
//Warning! this must be called 
// AFTER event_init and BEFORE event_dispatch
int learner_init(const char * config_file_path, custom_init_func cust_init, deliver_callback dcb);

//Prints statistic of learner events
//i.e. message loss, retransmission requests, etc
void learner_print_eventcounters();
