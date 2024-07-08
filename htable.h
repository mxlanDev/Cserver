#include "doublylinkedlist.h"

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
