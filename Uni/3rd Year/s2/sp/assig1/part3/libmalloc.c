#include <stdio.h> /* write */
#include <stdlib.h> /* heap */
#include <sys/mman.h> /* mmap/ sbrk() */
#include <unistd.h> /* size_t */
#include <stdbool.h> /* boolean */
#include "heapstats.h" /* heapstats */

bool verbose = false; /* for displaying errors */

bool malloc_init = false; /* initialising the free list */
int* free_list_pointers[7]; /* free lists for less than: 64, 128, 256, 512, 1024, 2048, 4096 */
int* prev_addr = NULL; /* points to last thing made in memory */
struct heapStats stats[1]; /* keeping track of stats */

/* copying stats over */
extern void SP2020_HeapStats(struct heapStats *in_stats)
{
	in_stats-> hs_heapBytes = stats-> hs_heapBytes;
	in_stats-> hs_allocatedBytes = stats-> hs_allocatedBytes;
	in_stats-> hs_allocatedChunks = stats-> hs_allocatedChunks;
	in_stats-> hs_freeChunks = stats-> hs_freeChunks;
	in_stats-> hs_metadataBytes = stats-> hs_metadataBytes; 
}

/* initialising pointer array */
void setup ()
{
	if(verbose) /* debugging */
	write (1, "setup init\n", 12);

	/* setting head to null */
	for (int i = 0; i < 7; ++i)
	{
		free_list_pointers[i] = NULL;
	}
	
	/* setting up heapstats */
	stats-> hs_heapBytes = 0; /* amount of mem that sbrk / mmap has allocated */
	stats-> hs_allocatedBytes = 0; /* amount of mem storing allocated data */
	stats-> hs_allocatedChunks = 0; /* amount of chunks storing allocated data */
	stats-> hs_freeChunks = 0; /* amount of chunks free */
	stats-> hs_metadataBytes = 0; /* amount of bytes used for metadata */

	if (verbose) /* debugging */
	write (1, "setup leave\n", 12);
	malloc_init = true; /* turning switch off */
}

/* malloc using sbrk and mmap */
void* malloc (size_t size)
{
	/* some c versions require variables declared at start */
	int** addr = NULL;
	int free_index = -1;
	int mem_size = size + 32;
	int chunk_size = 0;

	if (verbose) /* debugging */
	write (1, "malloc called\n", 14);
	
	/* setting up global variables on first instance */
	if (!malloc_init)
	{
		setup ();
	}
	
	/* if malloc (0) is called, do nothing*/
	if (size == 0)
	{
		return NULL;
	}
	/*  For small chunk, if the data can be stored in an already allocated free list
	    then use the free list ptr, instead of expanding the heap with sbrk.
	    For large chunk, use mmap (no free list pointer for this, just using munmap)
	*/
	
	/*  finding index of linked list and appropriate chunk size,
		based on data + header size
	*/
	switch (mem_size)
	{
		case 1 ... 64: 
		free_index = 0;
		chunk_size = 64;
		if (verbose) /* debugging */
		write (1, "free_index 0\n", 13);
		break;

		case 65 ... 128: 
		free_index = 1;
		chunk_size = 128;
		if (verbose) /* debugging */
		write (1, "free_index 1\n", 13);
		break;

		case 129 ... 256: 
		free_index = 2;
		chunk_size = 256;
		if (verbose) /* debugging */
		write (1, "free_index 2\n", 13);
		break;
		
		case 257 ... 512: 
		free_index = 3;
		chunk_size = 512;
		if (verbose) /* debugging */
		write (1, "free_index 3\n", 13);
		break;

		case 513 ... 1024: 
		free_index = 4;
		chunk_size = 1024;
		if (verbose) /* debugging */
		write (1, "free_index 4\n", 13);
		break;

		case 1025 ... 2048: 
		free_index = 5;
		chunk_size = 2048;
		if (verbose) /* debugging */
		write (1, "free_index 5\n", 13);
		break;

		case 2049 ... 4096: 
		free_index = 6;
		chunk_size = 4096;
		if (verbose) /* debugging */
		write (1, "free_index 6\n", 13);
		break;
	}

	/*  if index is not changed, then the data's size is 4096 or greater, 
		use big chunk
	*/
	if (free_index == -1)	
	{
		if (verbose) /* debugging */
		write (1, "big chunk\n", 10);

		int counter = 0;
		int leftovers = mem_size;
		while (leftovers > 0)
		{
			leftovers -= 4096;
			counter ++;
		}
		chunk_size = 4096 * counter;
		addr = (int**) mmap (NULL, chunk_size, PROT_WRITE | PROT_READ, MAP_ANON | MAP_PRIVATE, -1, 0);
		
		/* keeping track of stats */
		stats-> hs_heapBytes += chunk_size;
		stats-> hs_metadataBytes += 32;
	}


	/* if using free list */
	else if (free_list_pointers[free_index] != NULL)	
	{
		if (verbose) /* debugging */
		write (1, "using free list\n", 16);

		/* pop the free list element */
		addr = (int**) free_list_pointers[free_index];
		/* push the next element of the list to the top */
		free_list_pointers[free_index] = addr[1];

		/* keeping track of stats */
		stats-> hs_freeChunks --;
	}


	/* if allocating more memory to "heap" */
	else
	{
		if (verbose) /* debugging */
		write (1, "using sbrk()\n", 13);

		addr = (int**) sbrk (0);
		if (sbrk (chunk_size) == (void*) -1)
		{
			/* fault with sbrk() */
			if (verbose) /* debugging */
			write (1, "error leave malloc\n", 19);
			return NULL;
		}

		/* keeping track of stats */
		stats-> hs_heapBytes += chunk_size;
		stats-> hs_metadataBytes += 32;
	}

	/* pointers for next element in mem AND next element in free list */
	addr[0] = prev_addr;
	addr[1] = NULL;

	int* i_addr = (int*) addr; /* casting to int */
	/* stores the amount of data requested by malloc and the amount of data in the chunk (including header) */
	*( i_addr + 3) = size;
	*( i_addr + 4) = chunk_size;

	prev_addr = (int*) addr;
	addr += 4 ;

	/* calling malloc will always allocate at least 64 new bytes to the heap */
	stats-> hs_allocatedChunks ++;
	stats-> hs_allocatedBytes += chunk_size;
	
	if (verbose) /* debugging */
	write (1, "malloc leave\n", 13);

	return (void*) addr;
}

/* free address */
void free (void* addr)
{
	/* casting address to non-void */
	int* i_addr = (int*) addr;
	int** i_ptr_addr = (int**) addr;
	
	/* init variables */
	int** temp_ptr = NULL;	
	int free_index = -1;
	int block_size = -1;

	/* alligning pointers to start of header */
	i_addr = (i_addr - 8);
	i_ptr_addr = (i_ptr_addr - 4);

	/* getting block_size from header */
	block_size = *(i_addr + 4);
	if (verbose) /* dubuggin */
	write (1, "called free\n", 12);
	
	switch (block_size)
	{
		case 64: 
		free_index = 0;
		if (verbose) /* debugging */
		write (1, "free_index 0\n", 13);
		break;

		case 128: 
		free_index = 1;
		if (verbose) /* debugging */
		write (1, "free_index 1\n", 13);
		break;

		case 256: 
		free_index = 2;
		if (verbose) /* debugging */
		write (1, "free_index 2\n", 13);
		break;
		
		case 512: 
		free_index = 3;
		if (verbose) /* debugging */
		write (1, "free_index 3\n", 13);
		break;

		case 1024: 
		free_index = 4;
		if (verbose) /* debugging */
		write (1, "free_index 4\n", 13);
		break;

		case 2048: 
		free_index = 5;
		if (verbose) /* debugging */
		write (1, "free_index 5\n", 13);
		break;

		case 4096: 
		free_index = 6;
		if (verbose) /* debugging */
		write (1, "free_index 6\n", 13);
		break;
	}

	/* if small chunk */
	if (free_index != -1) 
	{
		/* push to front of list */
		i_ptr_addr[1] = free_list_pointers[free_index];
		free_list_pointers[free_index] = i_addr;

		/* O(amoun of heap elements) but can't think of better solution atm */
		temp_ptr = (int**) prev_addr;

		if (prev_addr != i_ptr_addr)
		{
			while (((int**) temp_ptr[0] == (int**) i_ptr_addr) && ((int**) temp_ptr != NULL))
			{
				temp_ptr = (int**) temp_ptr[0];
			}

			/* if not freeing anyting */
			if ((int**) temp_ptr == NULL)
			{
				if (verbose) /* debugging */
				{
					write (1, "no such ptr\n", 12);
					write (1, "leave free\n", 11);
				}
				return;
			}
		}

		/* removing addr from the heap */
		temp_ptr[0] = i_ptr_addr[0];

		/* updating stats */
		stats-> hs_freeChunks ++;
		stats-> hs_allocatedChunks --;
		stats-> hs_allocatedBytes -= block_size;
	}

	/* if large block (using mmap/munmap) */
	else
	{
		/* freeing memory using munmap */
		if (-1 == munmap ((void*) i_ptr_addr, block_size))
		{
			if (verbose) /* debugging */
			write (1, "error freeing large chunk\n", 26);
		}
		else
		{
			/* large data is not kept in a linked list, and is deleted from existence */
			stats-> hs_metadataBytes -= 32;
			stats-> hs_allocatedChunks --;
			stats-> hs_allocatedBytes -= block_size;
			stats-> hs_heapBytes -= block_size;
		}
	}
	
	if (verbose) /* debugging */
	write (1, "leave free\n", 11);

	return ;
}

/* realloc using predefinied malloc and free */
void* realloc (void* in_ptr, size_t size_1)
{
	/* fetching new pointer using malloc */
	int* addr = malloc (size_1);

	/* casting void ptr to int and int ptr */
	int* int_in_ptr = (int*) in_ptr;
	int* int_ptr_in_ptr = (int**) in_ptr;
	
	/* casting ptrs as chars, to do byte-wise operations */
	char* in_byte = (char*) in_ptr;
	char* out_byte = (char*) addr;

	/* variable init */
	int size_2 = *(addr - 4);
	int length = size_1;
	
	if (verbose) /* debugging */
	write (1, "called realloc\n", 15);

	/* finding the smaller allocation */
	if (size_2 < size_1)
		length = size_2;

	/* copying data over */
	for (int i = 0; i < length; ++i)
	{
		out_byte[i] = in_byte[i];
	}
	
	/* freeing original data */
	free (in_ptr);

	if (verbose) /* debugging */
	write (1, "leave realloc\n", 14);

	return (void*) addr;
}

/* calloc using malloc */
void* calloc (size_t ds_size, size_t num_elems)
{
	char* addr = malloc (ds_size*num_elems);

	if (verbose) /* debugging */
	write (1, "called calloc\n", 14);

	/* setting malloc'd data to zero */
	for (int i = 0; i < ds_size* num_elems; ++i)
	{
		addr[i] = 0;
	}

	if (verbose) /* debugging */
	write (1, "leave calloc\n", 13);

	return (void*) addr;
}