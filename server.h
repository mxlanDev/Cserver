#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>

#include "cache.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define SOCKERROR -1
#define THREAD_MAX 2
#define PATH_MAX 4096

enum recType{
  GET,
  POST,
  PUT,
  PATCH,
  DELETE,
};

typedef struct sockaddr_in SOCKIN;
typedef struct sockaddr SOCK;

typedef struct threadControl{
  pthread_mutex_t* mutex;
  volatile DList* queue;
}ThreadControl;

typedef struct clientInfo{
  int sockCli;
  Cache* serverCache;
  pthread_mutex_t* mutex;
}ClientInfo;



void* recHandle(void* ci);
void errHandle(int foo, char* msg);
void* threadLoop(void* args);
