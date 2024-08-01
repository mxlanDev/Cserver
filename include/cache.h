#include "htable.h"
#include "doublylinkedlist.h"

#include <string.h>

#define FNV_OFFSET_BASE ((unsigned int)0x811c9dc5)
#define FNV_PRIME ((unsigned int)0x01000193)

#define CACHE_MAX 20

typedef struct cell{
  char* path;
  FILE* fptr;
}Cell;

typedef struct cache{
  HTable* table;
  DList* list;
}Cache;

Cache* cacheInit();
FILE* cacheFile(Cache* cache, char* path);
FILE* cacheHit(HElement* element, DList* list);
FILE* cacheMiss(Cache* cache, char* path);
