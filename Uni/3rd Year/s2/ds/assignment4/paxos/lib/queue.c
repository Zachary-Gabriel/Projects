#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "paxos_config.h"
#include "lp_utils.h"
#include "lp_queue.h"

typedef struct queue_element_t {
    void * data;
    size_t size;
} queue_element;

struct lpqueue_t {
    unsigned queue_count, max_count;
    unsigned queue_size;
    unsigned read_index, write_index;
    queue_element queue[0];
};

lpqueue * queue_init(unsigned queue_length) {    
    if(queue_length == 0) {
        printf("Invalid queue size 0 for values manager.\n");
        return NULL;
    }
    
    // Create another CVM, this time with space for the queue
    lpqueue * q = calloc(1, sizeof(lpqueue) + (queue_length*sizeof(queue_element)));
    assert(q != NULL);
    
    // Clear queue
    q->max_count = queue_length;
    q->queue_count = 0;
    q->queue_size = 0;
    q->read_index = 0;
    q->write_index = 0;
    return q;
}

unsigned queue_get_current_size(lpqueue * q) {
	return q->queue_count;
}

bool queue_pop(lpqueue * q, void ** cmd_value_p, size_t * cmd_size_p) {
    if(q->queue_count == 0) {
        return VALUES_QUEUE_EMPTY;
    }

    queue_element * e = &q->queue[q->read_index];
    
    *cmd_value_p = e->data;
    *cmd_size_p = e->size;

    q->queue_count -= 1;
    q->queue_size -= e->size;

    //Increment read index
    q->read_index += 1;
    if(q->read_index == q->max_count) {
        q->read_index = 0;
    }
    return true;
}

bool queue_prepend(lpqueue * q, void * cmd_value, size_t cmd_size, bool make_copy) {
    bool result;
    //Only if the queue is full, drop the last element
    if(q->queue_count == q->max_count) {
        result = queue_discard_last(q);
        assert(result == true);
    }
    
    // Read index goes back one position
    if(q->read_index == 0) {
        q->read_index = (q->max_count - 1);
    } else {
        q->read_index -= 1;
    }
    
    //Add one element to the front
    q->queue_size += cmd_size;
    q->queue_count += 1;
    
    queue_element * e = &q->queue[q->read_index];
    e->size = cmd_size;
    
    if(make_copy) {
        e->data = malloc(cmd_size);
        memcpy(e->data, cmd_value, cmd_size);        
    } else {
        e->data = cmd_value;
    }
    
    
    return true;
}

bool queue_append(lpqueue * q, void * cmd_value, size_t cmd_size, bool make_copy) {
    //Queue is full!
    if(q->queue_count == q->max_count) {
        return false;
    }
    
    //Queue is too large
    if(q->queue_size + cmd_size > MAX_QUEUE_SIZE_BYTES) {
        return false;
    }

    //Add element to queue
    queue_element * e = &q->queue[q->write_index];
    e->size = cmd_size;
    if(make_copy) {
        e->data = malloc(cmd_size);
        memcpy(e->data, cmd_value, cmd_size);        
    } else {
        e->data = cmd_value;
    }

    q->queue_count += 1;
    q->queue_size += cmd_size;

    //Increment write index
    q->write_index += 1;
    if(q->write_index == q->max_count) {
        q->write_index = 0;
    }

    return true;
}

bool queue_discard_last(lpqueue * q) {
    //Queue is empty
    if(q->queue_count == 0) {
        return false;
    }
    
    // Write index goes back one position
    if(q->write_index == 0) {
        q->write_index = (q->max_count - 1);
    } else {
        q->write_index -= 1;    
    }
    
    //Delete element
    queue_element * e = &q->queue[q->write_index];
    q->queue_size -= e->size;
    q->queue_count -= 1;

    free(e->data);
    e->data = NULL;
    e->size = 0;
    return true;
}
