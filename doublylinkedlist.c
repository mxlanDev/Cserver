#include <stdio.h>
#include <stdlib.h>
#include "doublylinkedlist.h"


DList* dlistInit(){
  DList* list = (DList*)malloc(sizeof(DList));
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
  return list;
}

NodeT* initNode(void* data){
  NodeT* node = (NodeT*)malloc(sizeof(DList));
  node->data= data;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

NodeT* dlistFindNode(DList* list, int index){
  if(index>list->size||index<0) return NULL;
  NodeT* node;
  int position;
  int reverse;
  
  if(index>(list->size-1)/2){
    reverse = 1;
    node = list->tail;
    position = list->size-1;
  }
  else{
    reverse = 0;
    node = list->head;
    position = 0;
  }

  while(node!=NULL){
    if(position == index)break;
    node = (reverse ? (node->prev) : (node->next));
    position = (reverse ? (position-1) : (position + 1));
  }
  return node;
} 


void* dlistGetFirst(DList* list){
  if(list->head==NULL)return NULL;
  return list->head->data;
}

void* dlistGetLast(DList* list){
  if(list->tail==NULL)return NULL;
  return list->tail->data;
}


void* dlistGet(DList* list,int index){
  NodeT* node = dlistFindNode(list,index);
  if(node==NULL)return NULL;
  return node->data;
}

//Replaces data in node of index with value, returns previous data
void* dlistSet(DList* list, int index, void* value){
  NodeT* node = dlistFindNode(list,index); 
  if(node==NULL)return NULL;
  void* prevData = node->data;
  node->data = value;
  return prevData;
}

int dlistAdd(DList* list, int index, void* data){
  if(index>list->size)return -1;

  NodeT* newnode;
  NodeT* currnode;

  newnode = (NodeT*)malloc(sizeof(NodeT));
  if(list->size==0){
    list->head = newnode;
    list->tail = newnode;
    list->size++;
    return 0;
  }

  currnode = dlistFindNode(list,index);
  if(currnode!=NULL){
    newnode->prev = currnode->prev;
    newnode->next = currnode;
    if(currnode->next==NULL){
      list->head = newnode;
    }
    else{
      currnode->prev->next = newnode;
    }
    currnode->prev = newnode;

  }
  else{
    list->tail->next = newnode;
    newnode->prev = list->tail;
    list->tail = newnode;
  }
  list->size++;
  return 0;
}


int dlistPush(DList* list,void* data){
  NodeT* node = initNode(data);
  if(list->size == 0){
    list->tail = node;
  }
  else{
    list->head->prev = node;
    node->next = list->head;
  }
  list->head = node;
  list->size++;
  return 0;
}

int dlistPushTail(DList* list,void* data){
  NodeT* node = initNode(data);
  if(list->size == 0){
    list->head = node;
  }
  else{
    list->tail->next = node;
    node->prev = list->tail;
  }
  list->tail = node;
  list->size++;
  return 0;
}

void* dlistRemove(DList* list,int index){
  NodeT* node = dlistFindNode(list,index);

  if(node==NULL)return NULL;
  void* data = node->data;

  if(node->prev==NULL) list->head = node->next;
  else node->prev->next = node->next;

  if(node->next==NULL)list->tail = node->prev;
  else node->next->prev = node->prev;

  list->size--;
  free(node);
  return data;
}


void* dlistPop(DList* list){
  if(!list)return NULL;

  NodeT* node = list->head;
  if(node==NULL)return NULL;

  void* data = node->data;

  if(dlistRemove(list,0)== NULL)return NULL;
  
  return data;
}

void* dlistPopLast(DList* list){
  if(!list)return NULL;

  NodeT* node = list->tail;
  if(node==NULL)return NULL;

  void* data = node->data;

  if(dlistRemove(list,list->size-1)== NULL)return NULL;
  
  return data;
}
//implement if needed, list created for hash table only and free is done directly from key hash pairs
//void* dlistCLear(DList *list){} 
