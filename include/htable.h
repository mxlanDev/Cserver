#include <string.h>
#include <stdlib.h>

#define INITIAL_SIZE 10

typedef struct{
  char* key;
  unsigned int* hash;
  void* ptr;
}HElement;

typedef struct{
  HElement** array;
  unsigned int* size;
}HTable;

HTable* htableInit();
HElement* htableSearch(HTable* table, char* key, unsigned int (*hashFunction)(const char* key));
void htableAdd(HTable* table, char* key, void* ptr, unsigned int (*hashFunction)(const char* key));
void* htableRemove(HTable* table, char* key,  unsigned int (*hashFunction)(const char* key));
void htableExtend(HTable* table, unsigned int (*hashFunction)(const char* key));
