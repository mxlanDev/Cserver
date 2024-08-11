#include "server.h"
#include <unistd.h>

pthread_t threadPool[THREAD_MAX];
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//DList* queue;

int main(int argc, char** argv) {
  //sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);
  //queue = dlistInit();
  int sockServ,sockCli,addrSize;
  SOCKIN hostAddr, clientAddr;

  errHandle(sockServ = socket(AF_INET, SOCK_STREAM, 0),"Socket creation failed.");

  int opt = 1;
  if(setsockopt(sockServ,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0){
    printf("Reuseaddr not set");
  }

  printf("socket made\n");

  int hostAddrlen = sizeof(hostAddr);
  
  hostAddr.sin_family = AF_INET;
  hostAddr.sin_port = htons(PORT);
  hostAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  errHandle(bind(sockServ,(SOCK *)&hostAddr,hostAddrlen),"Binding socket failed.");
  printf("bound\n");

  errHandle(listen(sockServ,SOMAXCONN),"Priming listening failed.");// uses socket.h constant of 10, change if needed later
  printf("listening\n");
  
  int epfdArr[THREAD_MAX] = {0};

  for(int i = 0;i<THREAD_MAX;i++){
    int epfd = epoll_create1(0);
    epfdArr[i]=epfd; 
    pthread_create(&threadPool[i],NULL,threadLoop,&epfdArr[i]);
  }

  struct epoll_event event;
  int currentWorker = 0;
  
  while(1){
    //printf("Waiting...");
    addrSize = sizeof(SOCKIN);
    errHandle(sockCli = accept4(sockServ,(SOCK *)&clientAddr,(socklen_t *)&addrSize,SOCK_NONBLOCK),"Connection Failed.");
    int optval = 1;
    int keepAlive = setsockopt(sockCli, SOL_SOCKET, SO_KEEPALIVE, &optval,sizeof(optval));
    //printf("connected\n");
    event.events = EPOLLIN;
    event.data.fd = sockCli;

    epoll_ctl(epfdArr[currentWorker], EPOLL_CTL_ADD, sockCli, &event);
    currentWorker++;
    if(currentWorker==THREAD_MAX)currentWorker=0;
    printf("New Connection!\n");
  }
  return 0;
}
/*
void* threadLoop2(void* args){
  Cache* threadCache = cacheInit(); 
  while(1){
    int* sockCli;
    pthread_mutex_lock(&mutex);
    if(!(sockCli = (int*)dlistPop(queue))){
      pthread_cond_wait(&cond, &mutex);
      sockCli = (int*)dlistPop(queue);
    }

    pthread_mutex_unlock(&mutex);
    if(sockCli){
      recHandle(threadCache, *sockCli);
    }
  }
}
*/

void* threadLoop(void* args){
  Cache* threadCache = cacheInit();
  int threadEpfd = *(int*)args;
  return recHandle(threadCache,threadEpfd);
}

void* recHandle(Cache* cache, int threadEpfd){
  while(1){
    struct epoll_event polledEvents[EVENTS_MAX];
    int numReads = epoll_wait(threadEpfd, polledEvents, EVENTS_MAX, -1);
    for(int i = 0;i < numReads;i++){
      
      char buffer[BUFFER_SIZE];
      size_t bytes;
      int msgSize = 0;
      int sockCli = polledEvents[i].data.fd;

      while((bytes = read(sockCli,buffer+msgSize,sizeof(buffer)-msgSize-1))>0){
        msgSize += bytes;
        if(msgSize>BUFFER_SIZE-1||buffer[msgSize-1]=='\n')break;
      }

      if(msgSize==0){
        close(polledEvents[i].data.fd);
        printf("Connection closed!");
        continue;
      }
      //printf("PULSE\n");

      buffer[msgSize]=0;

      printf("%s",buffer);

      FILE *fp;

      enum httpStatus errorCode = OK;
      HttpRequest* request = loadRequest(buffer,&errorCode);
      HttpReply* reply = formReply(request,&errorCode);
      
      char* errorPage = statusToErrorPage(reply);
      if(reply->httpStatus>=400){
        fp = cacheFile(cache,errorPage);
        memset(reply->actualPath,0,PATH_MAX);
        strcpy(reply->actualPath,errorPage);
      }
      else fp = cacheFile(cache,reply->actualPath);
      free(errorPage);
      errorPage = NULL;
      
      if(fp == NULL){
        printf("Open error: %s \n",reply->actualPath);
        reply->httpStatus = 500;
        return NULL;
      }
      fseek(fp,0,SEEK_END);
      int size = ftell(fp);
      //char lengthBuffer[1024];
      //sprintf(lengthBuffer,"%d",size);
      //addReplyHeader(reply,"Content-Length",lengthBuffer);
      //printf("%s\n",lengthBuffer);
      
      addEntityHeaders(reply,size);

      sendReplyHeaders(reply,sockCli);
      
      sendReplyHeaders(reply,STDOUT_FILENO);
     
      fseek(fp,0,SEEK_SET);
      while((bytes = fread(buffer,1,BUFFER_SIZE,fp))>0){
        //printf("%zu bytes out\n", bytes);
        //write(STDOUT_FILENO,buffer,bytes);
        write(sockCli,buffer,bytes);
       }
      
      
      requestDelete(request);
      replyDelete(reply);

      //close(polledEvents[i].data.fd);
      //printf("Closing con");
    }
  }
  return NULL;
}

void errHandle(int foo, char* msg){
  if(foo==SOCKERROR){printf("%s",msg);exit(1);}
}


void printString(char* string){
  int i = 0;
  while(string[i]!='\0'){printchar(string[i]);i++;}
}

void printchar(unsigned char theChar) {

    switch (theChar) {

        case '\n':
            printf("\\n\n");
            break;
        case '\r':
            printf("\\r");
            break;
        case '\t':
            printf("\\t");
            break;
        default:
            if ((theChar < 0x20) || (theChar > 0x7f)) {
                printf("\\%03o", (unsigned char)theChar);
            } else {
                printf("%c", theChar);
            }
        break;
   }
}
