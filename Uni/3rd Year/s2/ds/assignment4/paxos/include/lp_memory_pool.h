#include <stdlib.h>
#include <stdbool.h>

// A simple memory pool for a limited number of objects of the same size

typedef struct memory_pool_t memory_pool;

// Creates a new memory pool
memory_pool * mempool_new(size_t num_entries, size_t entry_size);

// Destroy the memory pool
void mempool_destroy(memory_pool * mp);

// Request a slot from the memory pool
void * mempool_alloc(memory_pool * mp);
// Release a slot (which goes back in the free-list)
void mempool_free(memory_pool * mp, void * entry);

// Makes sure the given pointer is a valid entry of this memory pool
// (useful mostly for debugging)
bool is_mempool_chunk(memory_pool * mp, void * entry);

// Asks wether the memory pool is out of chunks
// (useful mostly for debugging)
bool is_mempool_exhaused(memory_pool * mp);
