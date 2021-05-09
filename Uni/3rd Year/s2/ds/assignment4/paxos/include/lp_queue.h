#include <stdbool.h>
#include <stdlib.h>

// A simple FIFO queue

#define VALUES_QUEUE_EMPTY false

typedef struct lpqueue_t lpqueue;

//Initialize a new queue
lpqueue * queue_init(unsigned queue_length);

// Get the queue head
bool queue_pop(lpqueue * q, void ** cmd_value_p, size_t * cmd_size_p);
// Add element at the end of the queue
bool queue_append(lpqueue * q, void * cmd_value, size_t cmd_size, bool make_copy);
// Add element before the current head
bool queue_prepend(lpqueue * q, void * cmd_value, size_t cmd_size, bool make_copy);
//Drop the last value appended
bool queue_discard_last(lpqueue * q);
//TODO queue destroy

//Get current number of entries in the queue
unsigned queue_get_current_size(lpqueue * q);
