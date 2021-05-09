#include <stdio.h> /* write */
#include <stdlib.h> /* heap */
#include <sys/mman.h> /* mmap/ sbrk() */
#include <unistd.h> /* size_t */
#include <stdbool.h> /* boolean */
#include "heapstats.h" /* heapstats */

bool verbose = false; /* for displaying errors */

bool malloc_init = false; /* initialising the free list */
int* free_list_pointers[8]; /* free lists for less than: 64, 128, 256, 512, 1024, 2048, 4096 , mmap big*/
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

/* Checking if the heap has been violated */
void canary_check ()
{
	/* Initialising variables */
	int** int_ptr_addr = (int**) prev_addr;
	char* byte_addr;
	int* int_addr;
	int i;
	int size;

	if (verbose) /* debugging */
	write (1, "canary check\n", 13);

	/* Checking canaries */	
	while (int_ptr_addr != NULL)
	{
		byte_addr = (char*) int_ptr_addr;
		int_addr = (int*) int_ptr_addr;

		size = int_addr[4];
		size += 32;

		/* checking for overflow */
		for (i = 0; i < 8; ++i)
		{	
			switch (i % 4)
			{
				case 3:
				if (byte_addr[size+i] != (char) 0xb1)
				{
					write (1, "buffer overflow\n", 16);
					abort ();
				}
				break;

				case 2:
				if (byte_addr[size+i] != (char) 0x6b)
				{
					write (1, "buffer overflow\n", 16);
					abort ();
				}
				break;

				case 1:
				if (byte_addr[size+i] != (char) 0x00)
				{
					write (1, "buffer overflow\n", 16);
					abort ();
				}
				break;

				case 0:
				if (byte_addr[size+i] != (char) 0xb5)
				{
					write (1, "buffer overflow\n", 16);
					abort ();
				}
				break;
			}
		}

		/* checking for underflow */
		if (int_addr[7] != 0xb16b00b5 || int_addr[6] != 0xb16b00b5)
		{
			write (1, "buffer underflow\n", 17);
			abort ();
		}

		/* going to next element in heap */
		int_ptr_addr = (int**) int_ptr_addr[0];
	}

	if (verbose) /* debugging */
	write (1, "leaving canary check\n", 21);
	return ;
}

/* checking if freed memory has been used */
void usage_after_free_check ()
{
	/* initialising variables */
	int** int_ptr_addr;
	int* int_addr;
	char* byte_addr;
	int i, size, j;
	
	if (verbose) /* debugging */
		write (1, "checking free use\n", 18);

	/* looping through each free list */
	for (i = 0; i < 7; ++i)
	{
		int_ptr_addr = (int**) free_list_pointers[i];
		while (int_ptr_addr != (int**) NULL)
		{
			byte_addr = (char*) int_ptr_addr;
			int_addr = (int*) int_ptr_addr;
			size = int_addr[5];
			size -= 32;

			for (j = 0; j < size; ++j)
			{
				switch (j % 4)
				{
					case 3:
					if (byte_addr[32+j] != (char) 0xb1)
					{
						write (1, "usage after free\n", 17);
						abort ();
					}
					break;

					case 2:
					if (byte_addr[32+j] != (char) 0x6b)
					{
						write (1, "usage after free\n", 17);
						abort ();
					}
					break;

					case 1:
					if (byte_addr[32+j] != (char) 0x00)
					{
						write (1, "usage after free\n", 17);
						abort ();
					}
					break;

					case 0:
					if (byte_addr[32+j] != (char) 0xb5)
					{
						write (1, "usage after free\n", 17);
						abort ();
					}
					break;
				}
			}

			/* going to next element in linked list */
			int_ptr_addr = (int**) int_ptr_addr[1];
		}
	}
	return ;
}

/* writting freed memory */
void rewrite_mem (void* addr)
{
	int** int_ptr_addr = (int**) addr;
	int_ptr_addr = (int_ptr_addr-4);
	int* int_addr = (int*) int_ptr_addr;
	char* byte_addr = (char*) int_ptr_addr;
	int chunk_size = int_addr[5];
	int_addr[4] = chunk_size - 8;
	int i;

	canary_check ();

	for (i = 0; i < chunk_size - 32; ++i)
	{
		switch (i % 4)
		{
			case 3:
			byte_addr[32+i] = (char) 0xb1;
			break;

			case 2:
			byte_addr[32+i] = (char) 0x6b;
			break;

			case 1:
			byte_addr[32+i] = (char) 0x00;
			break;

			case 0:
			byte_addr[32+i] = (char) 0xb5;
			break;
		}
	}
	return ;
}

/* checks if pointer is in heap */
bool ptr_in_heap (void* addr)
{
	int** temp_ptr = (int**) prev_addr;
	int** addr_ptr = (int**) addr;

	if (verbose) /*debugging*/
	write (1, "ptr_in_heap called\n",19);
	
	/* checking allocated */
	while (temp_ptr != (int**) NULL)
	{
		if (temp_ptr == addr_ptr)
		{
			return true;
		}
		temp_ptr = (int**) temp_ptr[0];
	}
	return false;
}

/* checks if pointer exists */
bool ptr_exists (void* addr)
{
	int i;
	int** temp_ptr = (int**) prev_addr;
	int** addr_ptr = (int**) addr;

	if (verbose) /*debugging*/
	write (1, "ptr_exists called\n",18);
	
	/* checking free */
	for (i = 0; i < 7; ++i)
	{
		temp_ptr = free_list_pointers[i];
		while (temp_ptr != NULL)
		{
			if ( temp_ptr == addr_ptr)
			{

				return true;
			}
			temp_ptr = (int**) temp_ptr[1];	
		}
	}
	
	/* checking large chunks metadata */
	temp_ptr = (int**) free_list_pointers[7];

	while (temp_ptr != NULL)
	{
		if (temp_ptr[4] == addr_ptr)
		{
			return true;
		}
		temp_ptr = (int**) temp_ptr[1];	
	}

	return false;
}

/* calling the mem test */
extern void SP2020_HeapTest ()
{
	canary_check ();
	usage_after_free_check ();
	return ;
}

/* initialising pointer array */
void setup ()
{
	if(verbose) /* debugging */
	write (1, "setup init\n", 12);

	/* setting head to null */
	for (int i = 0; i < 8; ++i)
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
	int free_index = 7;
	int mem_size = 32 + size + 8;
	int chunk_size = 0;
	
	/* checking if freed memory has been used */
	usage_after_free_check ();

	if (verbose) /* debugging */
	write (1, "malloc called\n", 14);
	
	/* setting up global variables on first instance */
	if (!malloc_init)
	{
		setup ();
	}
	
	/* if malloc (0) is called, do nothing */
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
	if (free_index == 7)	
	{
		int counter = 0;
		int leftovers = mem_size;

		if (verbose) /* debugging */
		write (1, "big chunk\n", 10);

		while (leftovers > 0)
		{
			leftovers -= 4096;
			counter ++;
		}

		chunk_size = 4096 * counter;
		addr = (int**) mmap (NULL, chunk_size, PROT_WRITE | PROT_READ, MAP_ANON | MAP_PRIVATE, -1, 0);

		/* keeping track of stats */
		stats-> hs_heapBytes += chunk_size;
		stats-> hs_metadataBytes += 40;
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
		stats-> hs_metadataBytes += 40;
	}

	/* pointers for next element in mem AND next element in free list */
	addr[0] = prev_addr;
	addr[1] = NULL;
	
	int* int_addr = (int*) addr; /* casting to int */
	/* stores the amount of data requested by malloc and the amount of data in the chunk (including header) */
	int_addr[4] = size;
	int_addr[5] = chunk_size;

	// underflow canary (8 bytes)
	int_addr[6] = 0xb16b00b5;	
	int_addr[7] = 0xb16b00b5;	

	prev_addr = (int*) addr;
	addr += 4 ;

	char* byte_addr = (char *) addr;

	// overflow canary (8 bytes)
	byte_addr[size+3] = 0xb1;
	byte_addr[size+2] = 0x6b;
	byte_addr[size+1] = 0x00;
	byte_addr[size+0] = 0xb5;

	byte_addr[size+7] = 0xb1;
	byte_addr[size+6] = 0x6b;
	byte_addr[size+5] = 0x00;
	byte_addr[size+4] = 0xb5;

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
	int* int_addr = (int*) addr;
	int** int_ptr_addr = (int**) addr;
	
	/* init variables */
	int** temp_ptr = NULL;	
	int free_index = -1;
	int block_size = -1;

	if (verbose) /* dubuggin */
	write (1, "called free\n", 12);

	/* If not freeing anything */
	if (addr == NULL)
	{
		if (verbose) /* dubuggin */
		write (1, "nullptr leave\n", 14);

		return ;
	}

	/* alligning pointers to start of header */
	int_addr = (int_addr - 8);
	int_ptr_addr = (int_ptr_addr - 4);

	/* if pointer DNE */
	if (!ptr_in_heap(int_ptr_addr))
	{
		if(!ptr_exists(int_ptr_addr))
		{
			write (1, "Invalid free\n", 13);
		}
		else
		{
			write (1, "double free\n", 12);
		}
		abort ();
	}
	/* getting block_size from header */
	block_size = *(int_addr + 5);

	switch (block_size)
	{
		case 64:
		free_index = 0;
		break;

		case 128:
		free_index = 1;
		break;

		case 256:
		free_index = 2;
		break;

		case 512:
		free_index = 3;
		break;

		case 1024:
		free_index = 4;
		break;

		case 2048:
		free_index = 5;
		break;

		case 4096:
		free_index = 6;
		break;
		
		default:
		free_index = 7;
	}

	/* O(amoun of heap elements) but can't think of better solution atm */
	temp_ptr = (int**) prev_addr;

	if (prev_addr != int_ptr_addr)
	{

		while (((int**) temp_ptr[0] != (int**) int_ptr_addr) && ((int**) temp_ptr[0] != (int**) NULL))
		{
			temp_ptr = (int**) temp_ptr[0];
		}

		/* if not freeing anything */
		if ((void*) temp_ptr[0] == NULL)
		{
			write (1, "double free\n", 12);
			abort ();
		}
	}
	else
	{
		prev_addr = (int*) temp_ptr[0];
	}

	/* if large block (using mmap/munmap) */
	if (free_index == 7)
	{ 
		int** temp_heap_ptr = int_ptr_addr[0];
		
		canary_check ();

		/* freeing memory using munmap */
		if (-1 == munmap ((void*) int_ptr_addr, block_size))
		{
			if (verbose) /* debugging */
			write (1, "error freeing large chunk\n", 26);
		}
		else
		{
			/* saving the metadata then deleting the chunk */
			temp_ptr[0] = temp_heap_ptr[0];

			int_ptr_addr = (int**) malloc (sizeof(int**));
			int** temp = (int**) addr;
			temp = temp-4;
			int_ptr_addr[0] = temp;
			int_ptr_addr = int_ptr_addr -4;

			stats-> hs_metadataBytes -= 40;
			stats-> hs_heapBytes -= block_size;
		}
	}
	else
	{
		temp_ptr[0] = int_ptr_addr[0];
		rewrite_mem (addr);
	}

	/* push to front of list */
	int_ptr_addr[1] = free_list_pointers[free_index];
	free_list_pointers[free_index] = int_ptr_addr;
	

	/* removing addr from the heap */
	// int_ptr_addr[0] = 0xb16b00b5;

	/* updating stats */
	stats-> hs_freeChunks ++;
	stats-> hs_allocatedChunks --;
	stats-> hs_allocatedBytes -= block_size;
	
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