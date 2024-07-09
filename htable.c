#include "htable.h"

HTable* htableInit(){
  HTable* table = (HTable*)malloc(sizeof(HTable));
  table->size = INITIAL_SIZE;
  HElement* array = (HElement*)calloc(INITIAL_SIZE,sizeof(HElement));
  return table;
}

HElement* htableSearch(HTable* table, char* key, unsigned int (*hashFunction)(const char* key)){
  unsigned int location = (*hashFunction)(key)%(unsigned int)table->size;
  return &table->array[location];
}
void htableAdd(HTable* table, char* key,void* ptr, unsigned int (*hashFunction)(const char* key)){
  unsigned int location = (*hashFunction)(key)%(unsigned int)table->size;
  if(table->array[location].ptr){
   
    return;
  }
  table->array[location].key = key;
  table->array[location].hash = location;
  table->array[location].ptr = ptr;
  return;
}

void* htableRemove(HTable* table, char* key, unsigned int (*hashFunction)(const char* key)){
  unsigned int location = (*hashFunction)(key)%(unsigned int)table->size;
  HElement element = table->array[location];
  if(table->array[location].ptr)
    if(*key==element->key&&!strcmp(key,element.key)){//???
      void* ptr = element.ptr;
      memset(table + location,0,sizeof(HElement));
      return ptr;
    }
  return NULL;
}

void htableExtend(HTable* table,unsigned int (*hashFunction)(const char* key)){
  unsigned int newSize = (table->size-1<<1)+1;
  HElement* new = (HElement*)calloc(newSize ,sizeof(HElement));
  for(int i = 0;i<table->size;i++){
    if(!table->array[i].ptr)continue;
    unsigned int location = (*hashFunction)(table->array[i].key)%(unsigned int)newSize;
    if(new[location].ptr){
      free(new);
      newSize = (newSize-1<<1)+1;
      new = (HElement*)calloc(newSize ,sizeof(HElement));
      i=0;
      continue;
    }
    new[location].ptr = table->array[i].ptr;
    new[location].key = table->array[i].key;
    new[location].hash = table->array[i].hash;
  }
  free(table->array);
  table->array = new;
}
