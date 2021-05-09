#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <event.h>

#include "lp_timers.h"
#include "lp_utils.h"
#include "forced_assert.h"

static struct timeval start_time;
static struct timeval end_time;
static periodic_event * pe;
static const unsigned num_wakeups = 500000;
static unsigned wakeups_count = 0;
static struct event terminate_event;

void end_test(int fd, short event, void * arg) {
	UNUSED_ARG(fd);
	UNUSED_ARG(event);
	UNUSED_ARG(arg);
	
	gettimeofday(&end_time, NULL);
	
	if(wakeups_count < num_wakeups) { 
		printf("(Max time elapsed)\n");
	}
	
	unsigned tot_time_msec;
	tot_time_msec = (end_time.tv_sec - start_time.tv_sec) * 1000;
	tot_time_msec += (end_time.tv_usec - start_time.tv_usec) / 1000;
	
	printf("%d wakeups in %.2f seconds\n", wakeups_count, ((float)tot_time_msec)/1000); 
	printf("%.2f wakeups per second\n", (((float)wakeups_count)/tot_time_msec)*1000);
	printf("%.2f microseconds between wakeups\n", ((float)tot_time_msec*1000)/wakeups_count);
	
	if((float)wakeups_count/tot_time_msec < 1) {
		printf("WARNING: The resolution of scheduled event is very low on this system!\n");
		printf("Consider switching to a realtime kernel [linux-rt] or increasing the CONFIG_HZ kernel parameter\n");
		exit(1);
	}
	
	printf("TEST SUCCESSFUL\n");
	exit(0);
}

void wakeup(void * arg) {
	UNUSED_ARG(arg);
	
	wakeups_count += 1;
	
	if(wakeups_count >= num_wakeups) {
		end_test(0, 0, NULL);
	}
}



int main (int argc, char const *argv[])
{

	UNUSED_ARG(argc);
	UNUSED_ARG(argv);

	event_init();
	
	struct timeval shortest_interval = {0, 1};
	
	gettimeofday(&start_time, NULL);
	
	pe = set_periodic_event(	
	    &shortest_interval,  /*Interval for this event*/
	    wakeup, /*Called periodically*/
	    NULL /*Argument passed to above function*/
	    );
	
	
	struct timeval max_duration = {5, 0};
	timeout_set(&terminate_event, end_test, NULL);
	event_add(&terminate_event, &max_duration);
	
	event_dispatch();
	
	return 0;
}
