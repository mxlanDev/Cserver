#include "cache.h"

Cache* cacheInit(){
  Cache* cache = (Cache*)malloc(sizeof(Cache));
  cache->list = dlistInit();
  cache->table = htableInit();
  return cache;
}

unsigned int cacheFnv(const char* path){
  unsigned char* s = (unsigned char*)path;
  unsigned int hash = FNV_OFFSET_BASE;
  while(*s){
    hash ^= (unsigned int)*s++;
    hash*=FNV_PRIME;
  }
  return hash;
}


FILE* cacheFile(Cache* cache, char* path){
  HElement* element = htableSearch(cache->table, path, cacheFnv);
  if(element)return cacheMiss(cache,path);
  return cacheHit(element,cache->list);
}

FILE* cacheHit(HElement* element,DList* list){
  NodeT* node = (NodeT*)element->ptr;
  Cell* cell =(Cell*)node->data;

  if(node==NULL)return NULL;
  void* data = node->data;

  if(node->prev==NULL) list->head = node->next;
  else node->prev->next = node->next;

  if(node->next==NULL)list->tail = node->prev;
  else node->next->prev = node->prev;
  
  if(list->size == 0){
    list->tail = node;
  }
  else{
    list->head->prev = node;
    node->next = list->head;
  }
  list->head = node;
  list->size++;

  return cell->fptr;
}

FILE* cacheMiss(Cache* cache, char* path){
  FILE* fptr = fopen(path,"r");
  Cell* cell;
  cell->path = path;
  cell->fptr = fptr;
  NodeT* node = dlistPush(cache->list,cell);
  htableAdd(cache->table,path,node,cacheFnv);
  if(cache->list->size>CACHE_MAX){
    Cell* burn = dlistPop(cache->list);
    htableRemove(cache->table,burn->path,cacheFnv);
  }
  return fptr;
}
