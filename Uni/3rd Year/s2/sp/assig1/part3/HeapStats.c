#include <stdint.h>
#include <stdio.h>
#include "heapstats.h"

void SP2020_HeapStats(struct heapStats *stats)
{
	stats-> hs_heapBytes = 3;
	stats-> hs_allocatedBytes = 0;
	stats-> hs_allocatedChunks = 0;
	stats-> hs_freeChunks = 0;
	stats-> hs_metadataBytes = 0;
	return ;
}