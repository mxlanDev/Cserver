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


#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
  char buffer[BUFFER_SIZE];
  char resp[] = "HTTP/1.0 200 OK\r\n"
  "Server: webserver-c\r\n"
  "Content-type: text/html\r\n\r\n"
  "<html>hello, world</html>\r\n";
  

  int sockfd = socket(AF_INET, SOCK_STREAM,0);
  if(sockfd == -1){
    printf("socked fucked up\n");
    return 1;
  }
  printf("socket made\n");
  struct sockaddr_in host_addr;
  int host_addrlen = sizeof(host_addr);
  
  host_addr.sin_family = AF_INET;
  host_addr.sin_port = htons(PORT);
  host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(sockfd,(struct sockaddr *)&host_addr,host_addrlen)!=0){
    printf("webserver bind fucked\n");
    return 0;
  }
  printf("bound\n");


  if(listen(sockfd,SOMAXCONN)!=0){
    printf("Listen Fuckup\n");
    return 1;
  }
  printf("listening\n");
 
  while(1){

    int client_fd = accept(sockfd,(struct sockaddr *)&host_addr,(socklen_t *)&host_addrlen);
    if (client_fd < 0) {
      printf("accept fucked\n");
      continue;
    }
    printf("connected\n");

    int valread = read(client_fd,buffer,256);
    if(valread < 0){
      printf("read fucked\n");
      continue;
    }
    //SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    //SSL* ssl = SSL_new(ctx);
    //SSL_set_fd(ssl,clientf_fd);

    int valwrite = write(client_fd,resp,strlen(resp));
    if(valwrite<0){
      printf("write fuckup\n");
      continue;
    }
    close(client_fd);
  }

  return 0;
}
