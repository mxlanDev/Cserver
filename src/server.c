#include "server.h"

pthread_t threadPool[THREAD_MAX];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
DList* queue;

int main(int argc, char** argv) {
  //sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);
  queue = dlistInit();
  int sockServ,sockCli,addrSize;
  SOCKIN hostAddr, clientAddr;

  ThreadControl* control = malloc(sizeof(ThreadControl));
  control->queue = queue;
  control->mutex = &mutex;

  for(int i = 0;i<THREAD_MAX;i++)pthread_create(&threadPool[i],NULL,threadLoop,NULL);

  errHandle(sockServ = socket(AF_INET, SOCK_STREAM, 0),"Socket creation failed.");

  printf("socket made\n");

  int hostAddrlen = sizeof(hostAddr);
  
  hostAddr.sin_family = AF_INET;
  hostAddr.sin_port = htons(PORT);
  hostAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  errHandle(bind(sockServ,(SOCK *)&hostAddr,hostAddrlen),"Binding socket failed.");
  printf("bound\n");

  errHandle(listen(sockServ,SOMAXCONN),"Priming listening failed.");// uses socket.h constant of 10, change if needed later
  printf("listening\n");
 
  while(1){
    printf("Waiting...");
    addrSize = sizeof(SOCKIN);
    errHandle(sockCli = accept(sockServ,(SOCK *)&clientAddr,(socklen_t *)&addrSize),"Connection Failed.");
    printf("connected\n");
    //recHandle(sockCli, serverCache);
    //pthread_t thr;

    pthread_mutex_lock(&mutex);
    dlistPush(queue,&sockCli);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    //pthread_create(&thr,NULL,recHandle,ci);
  }
  return 0;
}

void* threadLoop(void* args){
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

void* recHandle(Cache* cache, int sockCli){
  char buffer[BUFFER_SIZE];
  size_t bytes;
  int msgSize = 0;

  while((bytes = read(sockCli,buffer+msgSize,sizeof(buffer)-msgSize-1))>0){
    msgSize += bytes;
    if(msgSize>BUFFER_SIZE-1||buffer[msgSize-1]=='\n')break;
  }

  buffer[msgSize]=0;

  enum httpStatus errorCode = OK;
  HttpRequest* request = loadRequest(buffer,&errorCode);
  //printf("%d\n",errorCode);
  HttpReply* reply = formReply(request,&errorCode);
  //printf("%d\n",reply->httpStatus);
  sendReplyHeaders(reply,sockCli);

  FILE *fp;
  char* errorPage = statusToErrorPage(reply);
  //printf("%d\n",reply->httpStatus);
  if(reply->httpStatus>=400)fp = cacheFile(cache,errorPage);
  else fp = cacheFile(cache,reply->actualPath);
  //FILE *fp = fopen(actualpath,"r"); 

  if(fp == NULL){
    printf("Open error: %s \n",reply->actualPath);
    close(sockCli);
    return NULL;
  }

  while((bytes = fread(buffer,1,BUFFER_SIZE,fp))>0){
    printf("%zu bytes out", bytes);
    write(sockCli,buffer,bytes);
   }
   fseek(fp,0,SEEK_SET);
  
  requestDelete(request);
  replyDelete(reply);

  close(sockCli);
  printf("Closing con");
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
