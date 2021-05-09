#include "heapstats.h"
#include <stdio.h>
#include <stdlib.h>

int main() 
{
	write (1, "___________________\n", 20);
	struct heapStats hs = {0, 0, 0, 0, 0};

	int* a = malloc(sizeof(int));
	int* b = malloc(sizeof(int));
	int* c = malloc(sizeof(int));

	// printf("b size %u\n", b[4]);
	// b[1024] = 0xdeadbeef;
	// b[-1] = 0xdeadbeef;	
	free (a);
	// c[5] = 10;
	SP2020_HeapStats(&hs);
	SP2020_HeapTest();

	printf("heapBytes: %u\n", hs.hs_heapBytes);
	printf("allocatedBytes: %u\n", hs.hs_allocatedBytes);
	printf("allocatedChunks: %u\n", hs.hs_allocatedChunks);
	printf("freeChunks: %u\n", hs.hs_freeChunks);
	printf("metadataBytes: %u\n", hs.hs_metadataBytes); 
	return 0;
}
