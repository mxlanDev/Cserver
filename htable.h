#include "doublylinkedlist.h"

#define INITIAL_SIZE 10

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

HTable* htableInit();
HElement* htableSearch(HTable* table, char* key, unsigned int (*hashFunction)(const char* key));
void htableAdd(HTable* table, char* key, void* ptr, unsigned int *(hashFunction)(const char* key));
void htableRemove(HTable* table, char* key, void* ptr,  unsigned int *(hashFunction)(const char* key));
void htableExtend(HTable* table, char* key, void* ptr,  unsigned int *(hashFunction)(const char* key));
