#include "doublylinkedlist.c"
typedef struct{
  char* key;
  int hash;
  void* ptr;
}HElement;

typedef struct{
  HElement* array;
  int size;
  DList* list;
}HTable;

HTable* htableInit(){
  HTable* table = (HTable*)malloc(sizeof(HTable));
  table->size = 10;
  HElement* array = (HElement*)malloc(sizeof(HElement)*table->size);
  DList* list = dlistInit();
  return table;
} 


