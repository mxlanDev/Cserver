#include <stdio.h>
#include <stdlib.h>


typedef struct nodeT{
  void* data;
  struct nodeT* next;
  struct nodeT* prev;
} NodeT;

typedef struct{
  int size;
  NodeT* head;
  NodeT* tail;
} DList;

DList* dlistInit();
NodeT* initNode(void* data);
NodeT* dlistFindNode(DList* list, int index);
void* dlistGetFirst(DList* list);
void* dlistGetLast(DList* list);
void* dlistGet(DList* list,int index);
void* dlistSet(DList* list, int index, void* value);
int dlistAdd(DList* list, int index, void* data);
int dlistPush(DList* list,void* data);
int dlistPushTail(DList* list,void* data);
void* dlistRemove(DList* list,int index);
void* dlistPop(DList* list);
void* dlistPopLast(DList* list);
