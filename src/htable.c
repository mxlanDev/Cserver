#include "htable.h"

HTable* htableInit(){
  HTable* table = (HTable*)malloc(sizeof(HTable));
  table->size = malloc(sizeof(unsigned int));
  *table->size = INITIAL_SIZE;
  HElement** array = (HElement**)calloc(INITIAL_SIZE,sizeof(HElement*));
  table->array = array;
  return table;
}

HElement* htableSearch(HTable* table, char* key, unsigned int (*hashFunction)(const char* key)){
  unsigned int location = (*hashFunction)(key)%*table->size;
  return table->array[location];
}
void htableAdd(HTable* table, char* key,void* ptr, unsigned int (*hashFunction)(const char* key)){
  unsigned int location = (*hashFunction)(key)%*table->size;
  while(table->array[location]){ 
    htableExtend(table,hashFunction);
  }

  table->array[location] = (HElement*)malloc(sizeof(HElement));

  table->array[location]->key = key;
  table->array[location]->hash = malloc(sizeof(unsigned int));
  *(table->array[location]->hash) = location;
  table->array[location]->ptr = ptr;
  return;
}

void* htableRemove(HTable* table, char* key, unsigned int (*hashFunction)(const char* key)){
  unsigned int location = (*hashFunction)(key)%*table->size;
  HElement* element = table->array[location];
  if(table->array[location]->ptr)
    if(*key==*element->key&&!strcmp(key,element->key)){
      void* ptr = element->ptr;
      
      memset(table + location,0,sizeof(HElement));
      return ptr;
    }
  return NULL;
}

void htableExtend(HTable* table,unsigned int (*hashFunction)(const char* key)){
  unsigned int newSize = (*table->size-1<<1)+1;
  HElement** new = (HElement**)calloc(newSize ,sizeof(HElement*));
  for(int i = 0;i<*table->size;i++){
    if(!table->array[i])continue;
    unsigned int location = (*hashFunction)(table->array[i]->key)%newSize;
    if(new[location]){
      for(unsigned int i = 0;i<newSize;i++)if(!new[i])free(new[i]);
      free(new);
      newSize = (newSize-1<<1)+1;
      new = (HElement**)calloc(newSize ,sizeof(HElement*));
      i=0;
      continue;
    }
    new[location] = (HElement*)malloc(sizeof(HElement));

    new[location]->ptr = table->array[i]->ptr;
    new[location]->key = table->array[i]->key;
    new[location]->hash = table->array[i]->hash;
  }
  for(int i = 0;i<*table->size;i++)if(!table->array[i])free(table->array[i]);
  free(table->array);
  table->array = new;
  *table->size = newSize;
  return;
}
