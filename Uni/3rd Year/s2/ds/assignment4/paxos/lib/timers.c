#include <stdlib.h>
#include <assert.h>

#include "lp_timers.h"
#include "lp_utils.h"

struct periodic_event_t {
    struct timeval interval;
    void* cb;
    void* cb_arg;
    struct event ev; 
};

void
periodic_event_wrapper(int fd, short event, void *arg) {
    UNUSED_ARG(fd);
    UNUSED_ARG(event);
    
    //Arg is always struct periodic event
    periodic_event * pe = arg;

    // Invoke the real callback with saved arg
    periodic_event_callback cb = pe->cb;
    cb(pe->cb_arg);
    
    // Add the event
    int retval = 0;
    retval = event_add(&pe->ev, &pe->interval);
    assert(retval == 0);	    
}

periodic_event *  
set_periodic_event(
    struct timeval * interval,  /*Interval for this event*/
    periodic_event_callback cb, /*Called periodically*/
    void* arg   /*Argument passed to above function*/
    ) 
    {
        //Allocate struct with all info for this event
        periodic_event * p = calloc(1, sizeof(periodic_event));
        assert(p != NULL);
        
        
        // Set the event wrapper with the structure as arg
        evtimer_set(&p->ev, periodic_event_wrapper, p);
        evutil_timerclear(&p->interval);

        // Save callback and it's argument
        p->cb = cb;
        p->cb_arg = arg;
        
        // Copy the interval timeval 
        p->interval.tv_sec = interval->tv_sec;
        p->interval.tv_usec = interval->tv_usec;

        // Add the event
        int retval = 0;
        retval = event_add(&p->ev, &p->interval);
        assert(retval == 0);

        return p;
    }

/*
static void print_timeval(struct timeval * tv) {
	printf(" {%d:%ld} ", (int)(tv->tv_sec % 3600), (long int)tv->tv_usec);
}
*/


void timer_set_timeout(
    struct timeval * current_time, 
    struct timeval * variable, 
    struct timeval * interval) {
	
/*
		printf("curr: "); print_timeval(current_time);
		printf("\ninte: "); print_timeval(interval);
*/		
        variable->tv_sec = current_time->tv_sec + interval->tv_sec;
        int tot_usec = current_time->tv_usec + interval->tv_usec;
        if(tot_usec >= 1000000) {
            variable->tv_sec += 1;
            variable->tv_usec = tot_usec % 1000000;
        } else {
            variable->tv_usec = tot_usec;
        }
/*
		printf("\ndeadline: "); print_timeval(variable);
		printf("\n");
*/
}

bool timer_is_expired(struct timeval * deadline_time, struct timeval * curr_time) {  
	
/*
	printf("deadline: "); print_timeval(deadline_time);
	printf("\ncurrent : "); print_timeval(curr_time);
	printf("\n(%d) || ((%d) && (%d)) -> %d\n", 
	(deadline_time->tv_sec < curr_time->tv_sec),
	(deadline_time->tv_sec == curr_time->tv_sec),
	(deadline_time->tv_usec < curr_time->tv_usec),
	(deadline_time->tv_sec < curr_time->tv_sec) ||
        ((deadline_time->tv_sec == curr_time->tv_sec) &&
        (deadline_time->tv_usec < curr_time->tv_usec))
	);
*/	
    return (deadline_time->tv_sec < curr_time->tv_sec) ||
        ((deadline_time->tv_sec == curr_time->tv_sec) &&
        (deadline_time->tv_usec < curr_time->tv_usec));
}

