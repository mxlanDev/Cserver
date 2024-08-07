//#include <stdio.h>
//#include <stdlib.h>
#include <arpa/inet.h>
//#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <fcntl.h>
//#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <sys/epoll.h>

#include "cache.h"
#include "http.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define SOCKERROR -1
#define THREAD_MAX 5
#define EVENTS_MAX 16
#define PATH_MAX 4096

typedef struct sockaddr_in SOCKIN;
typedef struct sockaddr SOCK;

typedef struct threadControl{
  pthread_mutex_t* mutex;
  DList* queue;
}ThreadControl;

typedef struct clientInfo{
  int sockCli;
  Cache* serverCache;
}ClientInfo;




void* recHandle(Cache* cache, int sockCli);
void* recHandle2(Cache* cache, int sockCli);
void errHandle(int foo, char* msg);
void* threadLoop(void* args);
void printchar(unsigned char theChar);
void printstring(char* string);
