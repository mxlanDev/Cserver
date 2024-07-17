#include "server.h"


int main(int argc, char** argv) {
  sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);

  int sockServ,sockCli,addrSize;
  SOCKIN hostAddr, clientAddr;
  Cache* serverCache = cacheInit();
  volatile DList* queue = dlistInit();
  pthread_t threadPool[THREAD_MAX];
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  ThreadControl* control = malloc(sizeof(ThreadControl));
  control->queue = queue;
  control->mutex = &mutex;

  for(int i = 0;i<THREAD_MAX;i++)pthread_create(&threadPool[i],NULL,threadLoop,control);

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
    ClientInfo* ci = malloc(sizeof(ClientInfo));
    ci->sockCli = sockCli;
    ci->serverCache = serverCache;
    ci->mutex = &mutex;
    pthread_mutex_lock(&mutex);
    dlistPush(queue,ci);
    pthread_mutex_unlock(&mutex);
    //pthread_create(&thr,NULL,recHandle,ci);
  }
  return 0;
}

void* threadLoop(void* args){
  while(1){
    ClientInfo* ci;
    pthread_mutex_lock(((ThreadControl*)args)->mutex);
    ci = dlistPop(((ThreadControl*)args)->queue);
    pthread_mutex_unlock(((ThreadControl*)args)->mutex);
    if(ci){
      recHandle(ci);
    }
    sleep(1);
  }
}

void* recHandle(void* ci){
  int sockCli = ((ClientInfo*)ci)->sockCli;
  volatile Cache* serverCache = ((ClientInfo*)ci)->serverCache;
  pthread_mutex_t* mutex = ((ClientInfo*)ci)->mutex;
  free(ci);//Maybe changed later idk
  char buffer[BUFFER_SIZE];
  size_t bytes;
  int msgSize = 0;
  char actualpath[PATH_MAX+1];

  while((bytes = read(sockCli,buffer+msgSize,sizeof(buffer)-msgSize-1))>0){
    msgSize += bytes;
    if(msgSize>BUFFER_SIZE-1||buffer[msgSize-1]=='\n')break;
  }
  errHandle(bytes,"recv error");
  buffer[msgSize-1]=0;
  
  printf("REQ: %s\n",buffer);
  fflush(stdout);

  char path[BUFFER_SIZE]= {0};
  char request[64];
  sscanf(buffer,"%s %s",request,path);
  
  enum recType type;

  if(strcmp(request,"GET"))type = GET;

  if(type == 0){
    if(!path[1])strcpy(path,"/index.html");//strncpy better, change later
    

    if(strchr(path,'.')==0)strcat(path,".html");

    if(realpath(&(path[1]),actualpath)==NULL){
      printf("Bad path: %s\n",path);
      close(sockCli);
      return NULL;
    }

    pthread_mutex_lock(mutex);
    FILE *fp = cacheFile(serverCache,actualpath);
    pthread_mutex_unlock(mutex);

    if(fp == NULL){
      printf("Open error: %s \n",actualpath);
      close(sockCli);
      return NULL;
    }
     
    char resp[] = "HTTP/1.0 200 OK\r\n"
    "Server: webserver-c\r\n"
    "Content-type: text/html\r\n\r\n";

    write(sockCli,resp,strlen(resp));

    while((bytes = fread(buffer,1,BUFFER_SIZE,fp))>0){
      printf("%zu bytes out", bytes);
      write(sockCli,buffer,bytes);
    }
    fseek(fp,0,SEEK_SET);
  }
  close(sockCli);
  printf("Closing con");
  return NULL;
}

void errHandle(int foo, char* msg){
  if(foo==SOCKERROR){printf("%s",msg);exit(1);}
} 
