#ifndef __HEAPSTATS_H__
#define __HEAPSTATS_H__

#include <stdlib.h>

struct heapStats {
  size_t hs_heapBytes;
  size_t hs_allocatedBytes;
  size_t hs_allocatedChunks;
  size_t hs_freeChunks;
  size_t hs_metadataBytes;
};

void SP2020_HeapStats(struct heapStats *stats);

#endif // __HEAPSTATS_H__
