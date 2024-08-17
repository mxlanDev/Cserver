#include "http.h"
#include <stdio.h>

int stringToType(char* req){
  if(!strcmp(req,"GET"))return GET;
  if(!strcmp(req,"HEAD"))return HEAD;
  if(!strcmp(req,"POST"))return POST;
  if(!strcmp(req,"PUT"))return PUT;
  if(!strcmp(req,"PATCH"))return PATCH;
  if(!strcmp(req,"DELETE"))return DELETE;
  if(!strcmp(req,"CONNECT"))return CONNECT;
  if(!strcmp(req,"OPTIONS"))return OPTIONS;
  if(!strcmp(req,"TRACE"))return TRACE;
  return PROCESS_ERROR;
}

char* typeToString(enum httpMethod type){
  char* string = NULL;
  switch(type){
    case GET : string = "GET";break;
    case HEAD: string = "HEAD";break;
    case POST: string = "POST";break;
    case PUT: string = "PUT";break;
    case PATCH: string = "PATCH";break;
    case DELETE: string = "DELETE";break;
    case CONNECT: string = "CONNECT";break;
    case OPTIONS: string = "OPTIONS";break;
    case TRACE: string = "TRACE";break;
  }
  return string;
}

int stringToVersion(char* rec){
  if(!strcmp(rec,"HTTP/1.0"))return HTTP_10;
  if(!strcmp(rec,"HTTP/1.1"))return HTTP_11;
  if(!(strcmp(rec,"HTTP/2.0")&&strcmp(rec,"HTTP/2")))return HTTP_20;
  if(!(strcmp(rec,"HTTP/3.0")&&strcmp(rec,"HTTP/3")))return HTTP_30;
  return PROCESS_ERROR;
}

char* versionToString(enum httpVersion ver){
  char* string = NULL;
  switch(ver){
    case HTTP_10: string = "HTTP/1.0";break;
    case HTTP_11: string = "HTTP/1.1";break;
    case HTTP_20: string = "HTTP/2.0";break;
    case HTTP_30: string = "HTTP/3.0";break;
  }
  return string;
}
//Ugly blob of switch
char* codeToMessage(enum httpStatus code){
  switch(code){
    case 100: return "Continue";
    case 101: return "Switching Protocols";
    case 200: return "OK";
    case 201: return "Created";
    case 202: return "Accepted";
    case 203: return "Non-Authoritative Information";
    case 204: return "No Content";
    case 205: return "Reset Content";
    case 206: return "Partial Content";
    case 300: return "Multiple Choices";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 303: return "See Other";
    case 304: return "Not Modified";
    case 305: return "Use Proxy";
    case 306: return "(Unused)";
    case 307: return "Temporary Redirect";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 402: return "Payment Required";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 406: return "Not Acceptable";
    case 407: return "Proxy Authentication Required";
    case 408: return "Request Timeout";
    case 409: return "Conflict";
    case 410: return "Gone";
    case 411: return "Length Required";
    case 412: return "Precondition Failed";
    case 413: return "Request Entry Too Large";
    case 414: return "Request-URI Too Long";
    case 415: return "Unsupported Media Type";
    case 416: return "Request Range Not Satisfiable";
    case 417: return "Expectations Failed";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    case 502: return "Bad Gateway";
    case 503: return "Service Unavailable";
    case 504: return "Gateway Timeout";
    case 505: return "HTTP Version Not Supported";
    default: return NULL;
  }
}


char* extensionToMimeType(char* fileExtension){
  char* mimeType = calloc(50,1);
  if(!strcmp(fileExtension,".html")){strcpy(mimeType,"text/html");return mimeType;}
  if(!strcmp(fileExtension,".htm")){strcpy(mimeType,"text/html");return mimeType;}
  if(!strcmp(fileExtension,".css")){strcpy(mimeType,"text/css");return mimeType;}
  if(!strcmp(fileExtension,".ico")){strcpy(mimeType,"image/vnd.microsoft.icon");return mimeType;}
  if(!strcmp(fileExtension,".svg")){strcpy(mimeType,"image/svg+xml");return mimeType;}
  if(!strcmp(fileExtension,".png")){strcpy(mimeType,"image/png");return mimeType;}
  if(!strcmp(fileExtension,".jpg")){strcpy(mimeType,"image/jpeg");return mimeType;}
  //default
  printf("NOT WORK\n");
  strcpy(mimeType,"text/html");
  return mimeType;
}

char* statusToErrorPage(HttpReply* reply){
  char* path = calloc(18,1);
  sprintf(path,"errorPage%d.html",reply->httpStatus);
  return path;
}

HttpRequest* requestInit(){
  HttpRequest* request = calloc(1,sizeof(HttpRequest));
  request->headers = malloc(1);
  request->headerCount = 0;
  return request;
}

HttpReply* replyInit(){
  HttpReply* reply = calloc(1,sizeof(HttpReply));
  reply->headers = malloc(1);
  reply->headerCount = 0;
  reply->actualPath = calloc(PATH_MAX,1);
  return reply;
}

void requestDelete(HttpRequest* request){
  free(request->headers);
  free(request);
  return;
}
void replyDelete(HttpReply* reply){
  for(int i = 0;i<reply->headerCount;i++){
    free(reply->headers[i].key);
    free(reply->headers[i].value);
  }
  free(reply->headers);
  free(reply->actualPath);
  free(reply);
  return;
}

HttpRequest* loadRequest(char* clientInput, enum httpStatus* errorCode){
  HttpRequest* request = requestInit();
  char* doubleBreak;
  char* startingLine;
  char* headerLine;
  char* payload;

  char* outerSaveptr = NULL;
  char breakChars[4] = "\r\n\r\n";

  if(!(doubleBreak = strstr(clientInput,breakChars))){
    *errorCode = Bad_Request;
  }
 

  startingLine = strtok_r(clientInput,"\r\n" , &outerSaveptr);
  enum httpStatus tmp;
  *errorCode = assignErrorStatus(tmp=handleStartLine(request,startingLine), *errorCode);
  headerLine = strtok_r(NULL,"\r\n" , &outerSaveptr);
  while(headerLine&&headerLine<doubleBreak){
    *errorCode = assignErrorStatus(tmp=handleHeader(request,headerLine), *errorCode);
    headerLine = strtok_r(NULL,"\r\n" , &outerSaveptr); 
  }
  
  payload = doubleBreak+4;
  request->payload = payload;

  return request;
}

HttpReply* formReply(HttpRequest* request, enum httpStatus* errorCode){
  HttpReply* reply = replyInit();
  reply->version = HTTP_11;//hardcoded for now
  addGeneralHeaders(reply);
  if(*errorCode>=Bad_Request){
  reply->httpStatus = *errorCode;
  return reply;
  }
  switch(request->method){
    case GET : processGet(reply,request);break;
    case HEAD: processGet(reply, request);break;
    case POST: reply->httpStatus = Not_Implemented;break;
    case PUT: reply->httpStatus = Not_Implemented;break;
    case PATCH: reply->httpStatus = Not_Implemented;break;
    case DELETE: reply->httpStatus = Not_Implemented;break;
    case CONNECT: reply->httpStatus = Not_Implemented;break;
    case OPTIONS: reply->httpStatus = Not_Implemented;break;
  }
  return reply;
}

void processGet(HttpReply* reply, HttpRequest* request){
  char uri[BUFFER_SIZE] = {0};
  char* actualPath;

  strcpy(uri,request->uri);
  if(!uri[1])strcpy(uri,"/index.html");
  if(!strchr(uri,'.'))strcat(uri,".html");
  
  if(!(actualPath = realpath(&(uri[1]),NULL))){
    reply->httpStatus = Not_Found;
    return;
  }
  reply->httpStatus = OK;
  strcpy(reply->actualPath,actualPath);
  free(actualPath);
  return;
  //Chech uri
}

void sendReplyHeaders(HttpReply* reply, int fd){
  char* version = versionToString(reply->version);
  write(fd, version, sizeof(version));
  write(fd, " ", 1);
  char statusCode[3];
  sprintf(statusCode,"%d",reply->httpStatus);//change this if I form status code enums maybe
  write(fd, statusCode, 3);
  write(fd, " ", 1);
  char* statusMessage = codeToMessage(reply->httpStatus);
  write(fd,statusMessage,strlen(statusMessage));
  write(fd, "\r\n", 2);
  for(int i = 0;i<reply->headerCount;i++){
    write(fd,reply->headers[i].key,strlen(reply->headers[i].key));
    write(fd,": ",2);
    write(fd,reply->headers[i].value,strlen(reply->headers[i].value));
    write(fd,"\r\n",2);
  }
  write(fd, "\r\n", 2);
}

int handleStartLine(HttpRequest* request, char* startingLine){
  char* token;
  char* saveptr = NULL;

  enum httpStatus errorCode = OK;

  token = strtok_r(startingLine, " ", &saveptr);
  if(!(request->method = (enum httpMethod) stringToType(token))) assignErrorStatus(errorCode,Bad_Request);

  token = strtok_r(NULL, " ", &saveptr);
  if(!(request->uri = token)) assignErrorStatus(errorCode,Bad_Request);
  
  token = strtok_r(NULL, " ", &saveptr);
  if(!(request->version= (enum httpVersion) stringToVersion(token))) assignErrorStatus(errorCode,Bad_Request);
  if(request->version== HTTP_20)assignErrorStatus(errorCode, HTTP_Version_Not_Supported);
  
  if((token=strtok_r(NULL, " ", &saveptr)))assignErrorStatus(errorCode, Bad_Request);
  return errorCode;
}

enum httpStatus handleHeader(HttpRequest* request, char* startHeader){
  char* token;
  char* saveptr = NULL;

  enum httpStatus errorCode = OK;

  char* colonLocation = index(startHeader,':');
  *colonLocation = '\0';

  char* key = startHeader;
  char* value = colonLocation+1;
  errorCode = assignErrorStatus(addRequestHeader(request,key,value),errorCode);
  return errorCode;
}

int addRequestHeader(HttpRequest* request, char* key, char* value){
  request->headerCount++;
  HttpHeader* tmp =  realloc(request->headers, request->headerCount*sizeof(HttpHeader));
  if(!tmp){
    request->headerCount--;
    return Internal_Server_Error;
  }
  request->headers = tmp;
  request->headers[request->headerCount-1].key = key; 
  request->headers[request->headerCount-1].value = value;
  return OK;
}

int addReplyHeader(HttpReply* reply, char* key, char* value){
  reply->headerCount++;
  HttpHeader* tmp = realloc(reply->headers, reply->headerCount*sizeof(HttpHeader));
  if(!tmp){
    reply->headerCount--;
    return Internal_Server_Error;
  }
  reply->headers = tmp;
  char* headerKey = malloc(strlen(key)+1);
  char* headerValue = malloc(strlen(value)+1);
  sprintf(headerKey,"%s",key);
  sprintf(headerValue,"%s",value);
  headerKey[strlen(key)] = '\0';
  headerValue[strlen(value)] = '\0';
  reply->headers[reply->headerCount-1].key = headerKey; 
  reply->headers[reply->headerCount-1].value = headerValue;
  return OK;
}

void addGeneralHeaders(HttpReply* reply){
  addReplyHeader(reply,"Server","cserver");
  addReplyHeader(reply,"Connection","Keep-Alive");
}

void addEntityHeaders(HttpReply* reply,int contentLength){
  if(contentLength<0){
    printf("Something Baaaaaad hapened if this prints");
  }
  char lengthBuffer[BUFFER_SIZE];
  sprintf(lengthBuffer,"%d",contentLength);
  addReplyHeader(reply,"Content-Length",lengthBuffer);
  char* contentType = extensionToMimeType(rindex(reply->actualPath,'.'));
  addReplyHeader(reply,"Content-Type",contentType);
  free(contentType);
  return;
}

enum httpStatus assignErrorStatus(enum httpStatus ret, enum httpStatus error){
  if(ret == 500)return ret;
  if(error>= 400) return error;
  return ret;
}


