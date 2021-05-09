#include <string.h>
#include <time.h>
#include <assert.h>
#include <limits.h>

#include "lp_timers.h"
#include "lp_network.h" //TODO remove later if using submit proxy
#include "lp_topology.h" //TODO remove later if using submit proxy
#include "lp_submit_proxy.h"
#include "lp_learner.h"
#include "lp_utils.h"

#include "ringpaxos_messages.h"

/*
	This is an application implementing a Paxos learner AND also submitting values.
	It will try to keep CONCURRENT_VALUES outstanding values.
	When one of them is delivered, another one is submitted.
	Latency statistics are also collected for those values.
	If a value is not delivered for a certain amount of time after being submitted, 
	the application will submit it again.
	It is provided as an example.
*/

#define CONCURRENT_VALUES 80
#define MIN_VAL_SIZE (MAX_COMMAND_SIZE-50)
#define MAX_VAL_SIZE MAX_COMMAND_SIZE

#define MONITOR_LATENCY 1 //Set to 0 or 1 to turn off/on
#define LATENCY_VECTOR_SIZE 5000

static udp_sender * us = NULL;
static iid_t current_inst_number = 0;

typedef struct client_proposal_t {
	struct timeval submit_time;
	struct timeval timeout;
	submit_cmd_msg * submit_msg;
}client_proposal;

static struct timeval value_timeout_interval = {1, 0};
static struct timeval current_time;
static client_proposal pending_values[CONCURRENT_VALUES];
static char values_buffer[CONCURRENT_VALUES * (MAX_COMMAND_SIZE + sizeof(submit_cmd_msg))];
static periodic_event * periodic_check;

static struct timeval print_stats_interval = {5, 0};
periodic_event * periodic_stats;
int start_time;

long unsigned timeout_count = 0;
long unsigned deliver_count = 0;
long unsigned deliver_bytes = 0;

static void
generate_random_value(submit_cmd_msg * sm) {
	//Random Ascii printable char;
	char c = (random() % 94) + 33; 
	
	//Random size between min and max
	sm->cmd_size = (random() % (MAX_VAL_SIZE - MIN_VAL_SIZE)) + MIN_VAL_SIZE;
	
	memset(sm->cmd_value, c, sm->cmd_size);
	
	long * value_identifier = (long *)sm->cmd_value;
	value_identifier[0] = random();
	value_identifier[1] = random();	
}

static void 
submit_value(udp_sender * sender, submit_cmd_msg * msg) {
	net_send_udp(sender, msg, SUBMIT_CMD_MSG_SIZE(msg), client_submit);
}

static unsigned latency_samples_count = 0;
static long unsigned latency_samples_sum = 0;
static long unsigned min_latency = LONG_MAX;
static long unsigned max_latency = 0;

void save_submit_time(client_proposal * c, struct timeval * time_now) {
	if(!MONITOR_LATENCY) {
		return;
	}
	
	memcpy(&c->submit_time, time_now, sizeof(struct timeval));
}

void save_latency(struct timeval * submit_time, struct timeval * deliver_time) {
	if(!MONITOR_LATENCY) {
		return;
	}
	
	unsigned msec_diff = (deliver_time->tv_sec - submit_time->tv_sec)*1000;
	msec_diff += (deliver_time->tv_usec - submit_time->tv_usec)/1000;

	latency_samples_sum += msec_diff;
	latency_samples_count += 1;
	
	//Update min and max
	if(msec_diff > max_latency) {
		max_latency = msec_diff;
	}
	if(msec_diff < min_latency) {
		min_latency = msec_diff;
	}
}

void print_stats() {
	if(deliver_count > 0) {
		int elapsed_secs = time(NULL) - start_time;
		int delivery_rate = deliver_count/elapsed_secs;
		int delivery_tp = ((deliver_bytes * 8) / (1024*1024)) / elapsed_secs;

		printf("\n%lu vals (%lu kbytes) in %d sec\n", deliver_count, (deliver_bytes/1024), elapsed_secs);
		printf("%d val/s %d Mbit/s\n", delivery_rate, delivery_tp);
		printf("%lu timeouts (%lu%%)\n", timeout_count, ((timeout_count*100)/deliver_count));
	}
	
	if(MONITOR_LATENCY && latency_samples_count > 0) {
	
		double avg_latency = ((double)latency_samples_sum)/latency_samples_count;
		printf("Avg latency: %.2fms (sample min:%lu, max %lu)\n", 
			avg_latency, min_latency, max_latency);

		latency_samples_count = 0;
		latency_samples_sum = 0;
		max_latency = 0;
		min_latency = LONG_MAX;
	}	

	learner_print_eventcounters();
	
}

//Check that the sent values did not time-out
// The timed-out values are re-sent
void values_timeout_check(void* arg) {
	UNUSED_ARG(arg);
	gettimeofday(&current_time, NULL);
	
	client_proposal * c;
	unsigned i;
	for(i = 0; i < CONCURRENT_VALUES; i++) {
		c = &pending_values[i];
		if(timer_is_expired(&c->timeout, &current_time)) {
			timeout_count += 1;
			// printf("Value %u timed-out\n", i);
			submit_value(us, c->submit_msg);
			save_submit_time(c, &current_time);
			timer_set_timeout(&current_time, &c->timeout, &value_timeout_interval);
		}
	}
	
}

//Custom init for the learner
void client_pl_init() {
	printf("FastClient: submitting %d values of size [%lu...%lu]\n", 
		CONCURRENT_VALUES, MIN_VAL_SIZE, MAX_VAL_SIZE);
	
	
	//Use an udp sender instead of a submit_proxy
	us = udp_sender_init(
		lptopo_get_leader_addr(), 
		lptopo_get_leader_clients_port());
	// No autoflush, it's manual for this client
	assert(us != NULL);
	udp_sender_enable_autoflush(us, 25);
	
	//Periodically check for timeouts
	periodic_check =  
	set_periodic_event(
	    &value_timeout_interval,  /*Interval for this event*/
	    values_timeout_check, /*Called periodically*/
	    NULL /*Argument passed to above function*/
	    );

	//Periodically print statistics
	periodic_stats =  
	set_periodic_event(
	    &print_stats_interval,  /*Interval for this event*/
	    print_stats, /*Called periodically*/
	    NULL /*Argument passed to above function*/
	    );

	printf("Client initialization completed\n");
	start_time = time(NULL);
	
	gettimeofday(&current_time, NULL);

	//Submit the initial amount of values
	client_proposal * c;
	unsigned i;
	for(i = 0; i < CONCURRENT_VALUES; i++) {
		c = &pending_values[i];
		// c->submit_msg = malloc(sizeof(submit_cmd_msg) + MAX_VAL_SIZE);
		c->submit_msg = (submit_cmd_msg*) &values_buffer[i * (MAX_VAL_SIZE + sizeof(submit_cmd_msg))];
		generate_random_value(c->submit_msg);
		submit_value(us, c->submit_msg);
		save_submit_time(c, &current_time);
		timer_set_timeout(&current_time, &c->timeout, &value_timeout_interval);
	}
	
	
}

// Invoked when a value is delivered
void on_deliver(void* cmd_value, size_t cmd_size) {
	current_inst_number += 1;
	
	client_proposal * c;
	unsigned i;
	
	//Check if the delivered values was sent from this process.
	for(i = 0; i < CONCURRENT_VALUES; i++) {
		c = &pending_values[i];
		submit_cmd_msg * msg = c->submit_msg;
		
		//If so, submit another one
		if(cmd_size == msg->cmd_size &&
		memcmp(msg->cmd_value, cmd_value, cmd_size) == 0) {
			// printf("CMD %u accepted in instance %lu\n", i, current_inst_number);
			
			deliver_count += 1;
			deliver_bytes += cmd_size;

			gettimeofday(&current_time, NULL);
			save_latency(&c->submit_time, &current_time);
			
			generate_random_value(c->submit_msg);
			submit_value(us, c->submit_msg);
			save_submit_time(c, &current_time);
			timer_set_timeout(&current_time, &c->timeout, &value_timeout_interval);
			
			break;
		}
	}
}


int main (int argc, char const *argv[]) {

	// "clean" exit when a SIGINT (ctrl-c) is received
	enable_ctrl_c_handler();	
	
	srandom(1234);
	
    print_cmdline_args(argc, argv);
    
    // validate_acceptor_args_or_quit(argc, argv);
    const char * config_file_path = argv[1];
    
	event_init(); //Init libevent

	//Start the learner
	learner_init(config_file_path, client_pl_init, on_deliver); 
	
	//Enter libevent infinite loop
	event_dispatch();
    return 0;
}
