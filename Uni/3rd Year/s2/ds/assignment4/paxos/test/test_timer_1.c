#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "lp_timers.h"
#include "forced_assert.h"
#include "lp_utils.h"

int main (int argc, char const *argv[])
{
	
	UNUSED_ARG(argc);
	UNUSED_ARG(argv);
	
	struct timeval current_time;
	struct timeval interval;
	struct timeval timeout_variable;


	gettimeofday(&current_time, NULL);	
	interval.tv_sec = 1;
	interval.tv_usec = 0;
	
	timer_set_timeout(&current_time, &timeout_variable, &interval);
	assert(timer_is_expired(&timeout_variable, &current_time) == false);
	
	interval.tv_sec = 0;
	interval.tv_usec = 1;
	timer_set_timeout(&current_time, &timeout_variable, &interval);
	assert(timer_is_expired(&timeout_variable, &current_time) == false);

	gettimeofday(&current_time, NULL);	
	interval.tv_sec = 1;
	interval.tv_usec = 0;
	timer_set_timeout(&current_time, &timeout_variable, &interval);
	sleep(3);
	gettimeofday(&current_time, NULL);	
	assert(timer_is_expired(&timeout_variable, &current_time) == true);
	
	interval.tv_sec = 0;
	interval.tv_usec = 1;
	timer_set_timeout(&current_time, &timeout_variable, &interval);
	sleep(1);
	gettimeofday(&current_time, NULL);	
	assert(timer_is_expired(&timeout_variable, &current_time) == true);
	
	printf("TEST SUCCESSFUL!\n");
	return 0;
}
