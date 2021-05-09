#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "lp_queue.h"
#include "lp_memory_pool.h"
#include <lp_utils.h>

struct memory_pool_t {
	size_t tot_entries;
	size_t cur_entries;
	size_t entry_size;

	lpqueue * chunks_queue;	
	void * pool_begin; //Beginning of first entry
	void * pool_end; //Beginning of last entry
};

memory_pool * mempool_new(size_t num_entries, size_t entry_size) {
	memory_pool * mp = calloc(1, sizeof(struct memory_pool_t));
	mp->entry_size = entry_size;

	mp->tot_entries = num_entries;
	mp->cur_entries = num_entries;
	
	mp->chunks_queue = queue_init(num_entries);
	
	LOG_MSG(INFO, ("Mempool allocating %lu bytes (%lu Mbytes)", 
		(num_entries*entry_size), (num_entries*entry_size)/(1024*1024)));
	
	mp->pool_begin = calloc(num_entries, entry_size);
	
	unsigned i;
	bool success;
	
	char * next_chunk = mp->pool_begin;
	
	for(i = 0; i < num_entries; i++) {
		success = queue_append(mp->chunks_queue, next_chunk, entry_size, false);
		assert(success);
		next_chunk += entry_size;
	}
	
	mp->pool_end = (next_chunk - entry_size);
	
	return mp;
}

void mempool_destroy(memory_pool * mp) {
	if(mp->tot_entries != mp->cur_entries) {
		LOG_MSG(WARNING, ("WARNING: Destroying memory pool when not all chunks were released"));
	}
	free(mp->pool_begin);
	free(mp);
	
	//TODO queue_destroy
}

void * mempool_alloc(memory_pool * mp) {
	void * chunk_addr;
	size_t chunk_size;
	
	assert(mp->cur_entries > 0);
	mp->cur_entries -= 1;
	
	queue_pop(mp->chunks_queue, &chunk_addr, &chunk_size);
	assert(chunk_size == mp->entry_size);
	assert(chunk_addr >= mp->pool_begin && chunk_addr <= mp->pool_end);
	
	return chunk_addr;
}

void mempool_free(memory_pool * mp, void * entry) {
	
	assert(mp->cur_entries < mp->tot_entries);
	assert(entry >= mp->pool_begin && entry <= mp->pool_end);

	mp->cur_entries += 1;
	
	//Prepend for LIFO, better caching..?
	queue_prepend(mp->chunks_queue, entry, mp->entry_size, false);
		
}

bool is_mempool_chunk(memory_pool * mp, void * entry) {
	return(entry >= mp->pool_begin && entry <= mp->pool_end);
}

bool is_mempool_exhaused(memory_pool * mp) {
	return (mp->cur_entries == 0);
}
