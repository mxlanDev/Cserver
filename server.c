#include <errno.h>
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

void errHandle(int foo, char* msg);
void recHandle(int sockCli); 

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct sockaddr_in SOCKIN;
typedef struct sockaddr SOCK;

int main(int argc, char** argv) {
  char buffer[BUFFER_SIZE];
  char resp[] = "HTTP/1.0 200 OK\r\n"
  "Server: webserver-c\r\n"
  "Content-type: text/html\r\n\r\n"
  "<html>hello, world</html>\r\n";
  
  int sockServ,sockCli,addrSize;
  SOCKIN hostAddr, clientAddr;

  errHandle(sockServ = socket(AF_INET, SOCK_STREAM,0),"Socket creation failed.");

  printf("socket made\n");

  int hostAddrlen = sizeof(hostAddr);
  
  hostAddr.sin_family = AF_INET;
  hostAddr.sin_port = htons(PORT);
  hostAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  errHandle(bind(sockServ,(SOCK *)&hostAddr,hostAddrlen),"Binding socket failed.");
  printf("bound\n");

  errHandle(listen(sockServ,SOMAXCONN),"Priming listening failed.");
  printf("listening\n");
 
  while(1){
    printf("Waiting...");
    addrSize = sizeof(SOCKIN);
    errHandle(sockCli = accept(sockCli,(SOCK *)&clientAddr,(socklen_t *)&addrSize),"Connection Failed.");
    printf("connected\n");
    recHandle(sockCli);
    //SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    //SSL* ssl = SSL_new(ctx);
    //SSL_set_fd(ssl,clientf_fd);
  }
  return 0;
}

void recHandle(int sockCli){
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

  if(realpath(buffer,actualpath)==NULL){
    printf("Bad path: %s\n",buffer);
    close(sockCli);
    return;
  }

  FILE *fp = fopen(actualpath,"r");
  if(fp == NULL){
    printf("Open error: %s \n",buffer);
    close(sockCli);
    return;
  }

  while((bytes = fread(buffer,1,BUFFER_SIZE,fp))>0){
    printf("%zu bytes out", bytes);
    write(sockCli,buffer,bytes);
  }
  close(sockCli);
  fclose(fp);
  printf("Closing con");
}

void errHandle(int foo, char* msg){
  if(foo!=0)printf(msg);
} 
