// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/time.h>
// #include <unistd.h>

// #include "lp_timers.h"
#include "forced_assert.h"
#include "lp_memory_pool.h"
#include "lp_utils.h"

#define POOL_ENTRIES 10
#define POOL_ENTRY_SIZE 300

void write_chunk(void * data, char c) {
	unsigned i;
	char * datap = (char*)data;
	for(i = 0; i < POOL_ENTRY_SIZE; ++i) {
		datap[i] = c;
	}
}

void verify_chunk(void * data, char c) {
	unsigned i;
	char * datap = (char*)data;
	for(i = 0; i < POOL_ENTRY_SIZE; ++i) {
		assert(datap[i] == c);
	}
}

int main (int argc, char const *argv[])
{	
	UNUSED_ARG(argc);
	UNUSED_ARG(argv);

	unsigned i;
	
	memory_pool * mp = mempool_new(POOL_ENTRIES, POOL_ENTRY_SIZE);
	
	void * chunks[POOL_ENTRIES];

// Allocate all then free all
	for(i = 0; i < POOL_ENTRIES; ++i) {
		//Allocate chunk
		chunks[i] = mempool_alloc(mp);
		//Write into it
		write_chunk(chunks[i], (char)(i % 255));
	}
	
	for(i = 0; i < POOL_ENTRIES; ++i) {
		//Verify contents
		verify_chunk(chunks[i], (char)(i % 255));
		//Release
		mempool_free(mp, chunks[i]);
		chunks[i] = NULL;
	}

// Allocate all then free by using is_mempool_exhaused
	for(i = 0; is_mempool_exhaused(mp) == false; ++i) {
		//Allocate chunk
		chunks[i] = mempool_alloc(mp);
		//Write into it
		write_chunk(chunks[i], (char)(i % 255));
	}
	assert(i == POOL_ENTRIES);
	
	for(i = 0; i < POOL_ENTRIES; ++i) {
		//Verify contents
		verify_chunk(chunks[i], (char)(i % 255));
		//Release
		mempool_free(mp, chunks[i]);
		chunks[i] = NULL;
	}
	
	printf("TEST SUCCESSFUL!\n");
	return 0;
}


