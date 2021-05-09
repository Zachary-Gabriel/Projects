// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/time.h>
// #include <unistd.h>

// #include "lp_timers.h"
#include "forced_assert.h"
#include "lp_memory_pool.h"
#include "lp_utils.h"

#define POOL_ENTRIES 100
#define POOL_ENTRY_SIZE 300
#define NUM_OPERATIONS 5000000

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

void boundaries_test(memory_pool * mp) {
	
	unsigned i;
	void * chunk;
	for(i = 0; i < 100; ++i) {
		chunk = malloc(random() % (1024*1024));
		assert(is_mempool_chunk(mp, chunk) == false);		
	}
}

int main (int argc, char const *argv[])
{	
	UNUSED_ARG(argc);
	UNUSED_ARG(argv);

	unsigned i;
	
	memory_pool * mp = mempool_new(POOL_ENTRIES, POOL_ENTRY_SIZE);
	
	void * chunks[POOL_ENTRIES];
	for(i = 0; i < POOL_ENTRIES; i++) {
		chunks[i] = NULL;
	}

	srandom(1234);

	boundaries_test(mp);
	
	//Allocate half of the entries
	for(i = 0; i < POOL_ENTRIES; i += 2) {
		//Allocate chunk
		chunks[i] = mempool_alloc(mp);
		//Write into it
		write_chunk(chunks[i], (char)(i % 255));
	}
	
	//Randomly select an entry, 
	// if it is allocated, verify and free.
	// otherwise allocate and write
	for(i = 0; i < NUM_OPERATIONS; ++i) {
		int index = random() % POOL_ENTRIES;
		
		if(chunks[index] == NULL) {
			//Allocate chunk
			chunks[index] = mempool_alloc(mp);
			assert(is_mempool_chunk(mp, chunks[index]));
			
			//Write into it
			// printf("W: %d\n", index);
			write_chunk(chunks[index], (char)(index % 255));
		} else {
			//Verify contents
			// printf("R: %d\n", index);
			verify_chunk(chunks[index], (char)(index % 255));
			//Release
			mempool_free(mp, chunks[index]);
			chunks[index] = NULL;
		}	
	}
		
	printf("TEST SUCCESSFUL!\n");
	return 0;
}
