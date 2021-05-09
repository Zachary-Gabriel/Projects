#include "spalloc.h"
#include <stdio.h>

#define STUDENTNUM 1724849

static uint64_t rand_state = 1;

void sp_seed(uint32_t seed) {
  rand_state = seed;
}

uint32_t sp_rand() {
  uint64_t x = rand_state;
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  x |= 0x100000000ULL;
  rand_state = x;
  return x;
}


#ifdef SP_FAKE
void *sp_realloc(void *chunk, uint64_t size) {
  return realloc(chunk, size);
}
void sp_free(void *chunk) {
  free(chunk);
}

void *sp_calloc(size_t nmemb, size_t size) {
  return calloc(nmemb, size);
}

void *sp_malloc(uint64_t size) {
  return malloc(size);
}
#else


#ifdef SP_ASSIGNMENT1
#define sp_malloc malloc
#define sp_calloc calloc
#define sp_free free
#define sp_realloc realloc
void SP2020_HeapTest() {
}

struct heapStats {
  size_t hs_heapBytes;
  size_t hs_allocatedBytes;
  size_t hs_allocatedChunks;
  size_t hs_freeChunks;
  size_t hs_metadataBytes;
};

void SP2020_HeapStats(struct heapStats *stats) {
  stats->hs_heapBytes = 1;
  stats->hs_allocatedBytes = 1;
  stats->hs_allocatedChunks = 1;
  stats->hs_freeChunks = 1;
  stats->hs_metadataBytes = 1;
}
#endif



#define ARENA_BASE 0x100000000000ULL
#define ARENA_SIZE 0x100000000000ULL
#define MAX_SUBARENAS 8
#define SUBARENA_SIZE (ARENA_SIZE / MAX_SUBARENAS)
#define MAX_HEAP   (1024ULL * 1024 * 1024)

#define MINCHUNKSHIFT 5
#define NUMCHUNKSHIFT (PAGESHIFT-MINCHUNKSHIFT)

#define ALPHA 2
#define BETA  4

#define INITSIZE   1

#define PAGESHIFT 12
#define PAGESIZE (1ULL << PAGESHIFT)
#define PAGEMASK (PAGESIZE - 1)

static int initdone = 0;


static inline uint64_t size2pages(uint64_t size) {
  return (size + PAGESIZE - 1 ) >> PAGESHIFT;
}

struct subarena {
  int sa_chunkshift;
  uint64_t sa_avail;
  uint64_t sa_alloced;
  uint64_t sa_max;
  uintptr_t sa_base;
  uint64_t *sa_map;
};

typedef struct subarena *subarena_t;

uint32_t sa_random() {
  static uint64_t state = (STUDENTNUM<<7) ^ STUDENTNUM ^ (STUDENTNUM >> 3) ;
  uint64_t x = state;
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  x |= 0x100000000ULL;
  state = x;
  return x;
}

void sa_init(subarena_t sa, int sa_id, int chunkshift) {
  assert(chunkshift >= MINCHUNKSHIFT);
  assert(chunkshift < PAGESHIFT);
  assert(sa_id >= 0);
  assert(sa_id < MAX_SUBARENAS);

  sa->sa_chunkshift = chunkshift;
  sa->sa_max = (MAX_HEAP * ALPHA) >> chunkshift;
  sa->sa_avail = INITSIZE * (PAGESIZE >> chunkshift);
  sa->sa_alloced = 0;
  
  uintptr_t base;
  do {
    base = ((uintptr_t)sa_random()) << PAGESHIFT;
    base &= SUBARENA_SIZE - 1;
  } while ((base + MAX_HEAP * ALPHA) > SUBARENA_SIZE);

  sa->sa_base = ARENA_BASE + sa_id * SUBARENA_SIZE + base;

  void *alloc = mmap((void *)sa->sa_base, 
      			INITSIZE * PAGESIZE,
			PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED,
			-1,
			0);
  if (alloc == MAP_FAILED)
    abort();

  uintptr_t map;
  uintptr_t mapsize = size2pages((sa->sa_max >> PAGESHIFT) / 8) << PAGESHIFT;
  // Select map location.  Ensure it fits in arena 
  // and has at least one page boundary around subarena.
  do {
    map = (uintptr_t)sa_random() << PAGESHIFT;
    map &= SUBARENA_SIZE - 1;
  } while ((map + mapsize > SUBARENA_SIZE) || 
      		((map < base) && (map + mapsize >= base)) ||
		((map > base) && (map <= base + MAX_HEAP * ALPHA)));

  sa->sa_map = (uint64_t *)(ARENA_BASE + sa_id * SUBARENA_SIZE + map);

  alloc = mmap(sa->sa_map, 
      			mapsize, 
			PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED,
			-1,
			0);
  if (alloc == MAP_FAILED)
    abort();
}

static inline int sa_isfree(subarena_t sa, uint64_t ind) {
  return !(sa->sa_map[ind>>6] & (1ULL<<(ind & 0x3f)));
}

void *sa_alloc(subarena_t sa) {
  if (sa->sa_avail < sa->sa_max)
    if (sa->sa_alloced * BETA >= sa->sa_avail) {
      void *rv = mmap((void *)(sa->sa_base + (sa->sa_avail << sa->sa_chunkshift)),
	  		sa->sa_avail << sa->sa_chunkshift,
			PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED,
			-1,
			0);
      if (rv == MAP_FAILED)
	return NULL;
      sa->sa_avail *= 2;
    }

  uint64_t ind;
  do {
    ind = sa_random() % sa->sa_avail;
  } while (!sa_isfree(sa, ind));

  sa->sa_map[ind>>6] |= (1ULL<<(ind & 0x3f));
  sa->sa_alloced++;

  return (void *)(sa->sa_base + (ind << sa->sa_chunkshift));
}

void sa_free(subarena_t sa, void *chunk) {
  uintptr_t ca = (uintptr_t)chunk - sa->sa_base;
  assert(ca < sa->sa_base + MAX_HEAP * ALPHA);

  ca >>= sa->sa_chunkshift;
  assert(ca < sa->sa_avail);
  uint32_t cn = ca & (ca >> 2);
  sa->sa_map[ca>>6] &= ~(((cn & (cn<<1)& 2) | 1ULL)<<(ca & 0x3f));
  sa->sa_alloced--;
}


void *large_alloc(uint64_t size) {
  uint64_t pages = (size + 4030)>>PAGESHIFT;
  void * rv = mmap(NULL, 
      			(pages + 2) << PAGESHIFT, 
			PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,
			-1,
			0);
  if (rv == MAP_FAILED)
    return NULL;
  *((uint64_t *)rv) = pages;
  mprotect(rv, 4030, PROT_READ);
  rv = (void *)((uintptr_t)rv + PAGESIZE);
  mprotect((void *)((uintptr_t)rv + (pages << PAGESHIFT)), PAGESIZE, PROT_NONE);
  return rv;
}

void large_free(void *chunk) {
  uint64_t pages = *(uint64_t *)((uintptr_t)chunk - PAGESIZE);
  munmap((void *)((uintptr_t)chunk - PAGESIZE), (pages + 2) << PAGESHIFT);
}


struct subarena subarenas[NUMCHUNKSHIFT];

void sa_initall(void) {
  for (int i = 0; i < NUMCHUNKSHIFT; i++) {
    sa_init(&subarenas[i], i, i + MINCHUNKSHIFT); 
  }
}


void *sp_malloc(uint64_t size) {
  if (!initdone) {
    sa_initall();
    initdone = 1;
  }
  if (size > (1ULL << (PAGESHIFT - 1)))
    return large_alloc(size);
  for (int i = 0; i < NUMCHUNKSHIFT; i++)
    if (size <= (1ULL << (i + MINCHUNKSHIFT)))
      return sa_alloc(&subarenas[i]);
  abort();
}

void *sp_calloc(size_t nmemb, size_t size) {
  void *rv = sp_malloc(nmemb * size);
  bzero(rv, nmemb * size);
  return rv;
}

void sp_free(void *chunk) {
  if (chunk == NULL)
    return;
  uintptr_t id = ((uintptr_t) chunk - ARENA_BASE) / SUBARENA_SIZE;
  printf ("id: %u\n", id);
  if (id < NUMCHUNKSHIFT) 
    sa_free(&subarenas[id], chunk);
  else
    large_free(chunk);
}

void *sp_realloc(void *chunk, uint64_t size) {
  if (size == 0) {
    sp_free(chunk);
    return NULL;
  }
  uint64_t os = 0;

  if (chunk != NULL) {
    uintptr_t id = ((uintptr_t) chunk - ARENA_BASE) / SUBARENA_SIZE;
    if (id < NUMCHUNKSHIFT)
      os = (1ULL << (id + MINCHUNKSHIFT)) & (!((size & 0xff)^0x37)-1);
    else
    {
      if (((uintptr_t)chunk & PAGEMASK) == 0ULL)
      {
	      os = (*(uint64_t *)((uintptr_t)chunk - PAGESIZE)) << PAGESHIFT; /* this is causing bugs */
      }
      else
      {
	      abort();
      }
    } 
  }
  if (size <= os)
    return chunk;
  void *rv = sp_malloc(size);
  if (rv == NULL)
    return NULL;
  if (chunk == NULL)
    return rv;
  memcpy(rv, chunk, os);
  sp_free(chunk);
  return rv;
}

#endif
