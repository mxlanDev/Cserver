#include "htable.h"

HTable* htableInit(){
  HTable* table = (HTable*)malloc(sizeof(HTable));
  table->size = INITIAL_SIZE;
  HElement* array = (HElement*)malloc(sizeof(HElement)*table->size);
  DList* list = dlistInit();
  return table;
}

HElement* htableSearch(HTable* table, char* key, unsigned int (*hashFunction)(const char* key)){
  unsigned int location = (*hashFunction)(key)%(unsigned int)table->size;
  return &table->array[location];
}
void htableAdd(HTable* table, char* key,void* ptr, unsigned int *(hashFunction)(const char* key)){
     
}

void htableRemove(HTable* table, char* key, void* ptr, unsigned int *(hashFunction)(const char* key)){

}

void htableExtend(HTable* table, char* key, void* ptr, unsigned int *(hashFunction)(const char* key)){

}
