#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include "paxos_config.h"
#include "lp_utils.h"
#include "lp_submit_proxy.h"

#include "ringpaxos_messages.h"

/*
	This is the simplest application submitting values 
	at a fixed rate using a submit_proxy.
	It is provided as an example.
*/

#define RAND_SEED 1234 //Use (time(NULL) % 1000) for random seed
#define MIN_VAL_SIZE (MAX_COMMAND_SIZE-50)
#define MAX_VAL_SIZE MAX_COMMAND_SIZE
#define NUM_VALUES 500000
#define SLEEP_INTERVAL_USEC 1000
#define SUBMIT_PER_WAKE  16
#define GIGABYTE (1024*1024*1024)
#define PRINT_INTERVAL_BYTES (GIGABYTE)

static const bool verbose = false;

//Submit packet static buffer
static char smsg_buf[MAX_MESSAGE_SIZE];
static submit_cmd_msg * smsg = (submit_cmd_msg*)&smsg_buf;

submit_proxy * sp;


//Initialization
bool client_init(const char * config_path) {
	
	//Create a submit proxy
	sp = submit_proxy_udp_init(config_path);
	assert(sp != NULL);

	return true;
}

//Generate a random value in the given buffer
static size_t 
generate_random_val(void * buf) {
	//Random Ascii printable char;
	char c = (random() % 94) + 33; 
	
	//Random size between min and max
	size_t s = (random() % (MAX_VAL_SIZE - MIN_VAL_SIZE)) + MIN_VAL_SIZE;
	
	memset(buf, c, s);
	return s;
}

// Loop until the given number of values was sent:
// submit and then sleep for a while
void submit_loop() {
	int count = 0;
	long unsigned bytes_count = 0;
	
	struct timeval time_now;
	struct timeval time_prev;
	
	gettimeofday(&time_prev, NULL);
	
	while(count < NUM_VALUES) {

		// Submit SUBMIT_PER_WAKE messages
		unsigned i;
		for(i = 0; i < SUBMIT_PER_WAKE; i++) {
			count += 1;		
			smsg->cmd_size = generate_random_val(&smsg->cmd_value);
			
			submit_command(sp, smsg);

			bytes_count += smsg->cmd_size;
			
			if(verbose) {

				char * str = (char*)&smsg->cmd_value;
				printf("* Submit %d (size:%lu)\n", count, smsg->cmd_size);
				printf("* [%c, %c, %c, ...] <<< \n", str[0], str[1], str[2]);
			}
		}
		
		//Force flushing of the current packet
		submit_proxy_flush_now(sp);
		
		//Print some statistics
		if(bytes_count >= PRINT_INTERVAL_BYTES) {
			long unsigned megabit = ((bytes_count*8)/(1024*1024));
			gettimeofday(&time_now, NULL);
			long unsigned millisec = (time_now.tv_sec - time_prev.tv_sec) * 1000;
			millisec += ((time_now.tv_usec - time_prev.tv_usec)/1000);
			float mbps = (((float)megabit)/millisec*1000);
			
			printf("Sent %lu Megabits in %lu milliseconds -> %.2f Mbps\n",
			 	megabit, millisec, mbps);
			
			memcpy(&time_prev, &time_now, sizeof(struct timeval));
			bytes_count = 0;
		}
		
		//Go to sleep for a while
		usleep(SLEEP_INTERVAL_USEC);
	}
}


int main (int argc, char const *argv[]) {
	
	if(argc != 2) {
		printf("Wrong arguments:\n");
		printf("%s config_file_path\n", argv[0]);
		exit(1);
	}

	
	enable_ctrl_c_handler();
	
	if(client_init(argv[1])) {
		unsigned avg_val_size = (MAX_VAL_SIZE + MIN_VAL_SIZE)/2;
		unsigned wps = (1000000/SLEEP_INTERVAL_USEC);
		unsigned vps = wps*SUBMIT_PER_WAKE;
		unsigned MBps = (vps*avg_val_size)/(1024*1024);
		
		printf("(Theoretical) Submit rate is: %u val/s, %u Mbit/s\n", vps, MBps*8);
		
		submit_loop();		
	} else {
		printf("Initialization failed!\n");
		exit(1);
	}
	return 0;
}
