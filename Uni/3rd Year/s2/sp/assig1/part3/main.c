#include "heapstats.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	write (1, "___________________\n", 20);
	struct heapStats hs = {0, 0, 0, 0, 0};
	
	int* a = (int*) malloc (sizeof(int));
	a[0] = 9;
	int* b = (int*) malloc (sizeof(int));
	int* c = (int*) realloc (a, sizeof(int)*2);
	printf("%u\n", c[0]);
	int* d = (int*) malloc (sizeof(int));

	// free (b);
	free (c);

	int* e = (int*) calloc (1, sizeof(int));
	printf("%u\n", e[0]);
	int* f = (int*) malloc (sizeof (int) * 4000);
	SP2020_HeapStats (&hs);
	printf("%u, %u, %u, %u, %u\n", hs.hs_heapBytes, hs.hs_allocatedBytes, hs.hs_allocatedChunks, hs.hs_freeChunks, hs.hs_metadataBytes);
	return 0;
}
