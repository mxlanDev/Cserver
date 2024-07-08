#include "htable.h"

HTable* htableInit(){
  HTable* table = (HTable*)malloc(sizeof(HTable));
  table->size = 10;
  HElement* array = (HElement*)malloc(sizeof(HElement)*table->size);
  DList* list = dlistInit();
  return table;
} 


