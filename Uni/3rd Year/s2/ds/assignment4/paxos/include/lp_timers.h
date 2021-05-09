#include <stdbool.h>
#include <sys/time.h>
#include <event.h>

// Timer utilities on top of libevent

struct periodic_event_t; 
typedef struct periodic_event_t periodic_event; 

typedef void(*periodic_event_callback)(void*);

//Set a periodic event, (i.e. automatically triggered periodically)
periodic_event *  
set_periodic_event(
    struct timeval * interval,  /*Interval for this event*/
    periodic_event_callback cb, /*Called periodically*/
    void* arg /*Argument passed to above function*/
    );

//Timeval arithmetics, sets the deadline for a timer
// based on current time + timeout time
void timer_set_timeout(struct timeval * current_time, struct timeval * variable, struct timeval * interval);

// Checks wether some timeout has expired
bool timer_is_expired(struct timeval * deadline_time, struct timeval * curr_time);
