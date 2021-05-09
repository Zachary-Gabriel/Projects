#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#include "event.h"

#include "lp_timers.h"
#include "forced_assert.h"
#include "lp_utils.h"

// void wait_a_while(int seconds) {
// 	int enter_time = time(NULL);
// 	while(1) {
// 
// 		unsigned i;
// 		for(size_t i = 0; i < 1000; ++i) {
// 			usleep(1000);
// 		}
// 		
// 		if(time() - enter_time >= seconds) {
// 			break;
// 		}
// 	}
// }

static int wakeup_count = 0;
static int enter_time;

static void callback(void * arg) {
	assert(arg == &wakeup_count);
	int * wakeup_count_p = (int*)arg;
	*wakeup_count_p += 1;
	
	if(*wakeup_count_p >= 250) {
		printf("TEST SUCCESSFUL\n");
		exit(0);
	} else if (time(NULL) - enter_time > 3) {
		//event dispatching is way too slow!
		printf("TEST FAILED (event triggered %d times)\n", *wakeup_count_p);
		exit(1);
	}
	
}

int main (int argc, char const *argv[])
{
	UNUSED_ARG(argc);
	UNUSED_ARG(argv);
	
	event_init();

	struct timeval interval;
	interval.tv_sec = 0;
	interval.tv_usec = 10000; //10msec

	periodic_event * pe;
	
	pe = set_periodic_event(&interval, callback, &wakeup_count);
	
	// wait_a_while(3);
	
	enter_time = time(NULL);
	
	event_dispatch();
	return 0;
}
