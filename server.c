#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <string.h>

void errHandle(int foo, char* msg);

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

    int valread = read(sockCli,buffer,256);
    if(valread < 0){
      printf("read fucked\n");
      continue;
    }
    //SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    //SSL* ssl = SSL_new(ctx);
    //SSL_set_fd(ssl,clientf_fd);

    int valwrite = write(sockCli,resp,strlen(resp));
    if(valwrite<0){
      printf("write fuckup\n");
      continue;
    }
    close(sockCli);
  }

  return 0;
}

void errHandle(int foo, char* msg){
  if(foo!=0)printf(msg);
} 
