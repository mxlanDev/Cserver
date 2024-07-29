#include "http.h"

int stringToType(char* req){
  if(!strcmp(req,"GET"))return GET;
  if(!strcmp(req,"HEAD"))return HEAD;
  if(!strcmp(req,"POST"))return POST;
  if(!strcmp(req,"PUT"))return PUT;
  if(!strcmp(req,"PATCH"))return PATCH;
  if(!strcmp(req,"DELETE"))return DELETE;
  if(!strcmp(req,"CONNECT"))return CONNECT;
  if(!strcmp(req,"OPTIONS"))return OPTIONS;
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
char* codeToMessage(unsigned int code){
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



HttpRequest* loadRequest(char* clientInput, int* errorCode){
  HttpRequest* request;
  char* doubleBreak;
  char* startingLine;
  char* headerLine;
  char* payload;

  doubleBreak = strstr(clientInput,"\r\n\r\n");
  startingLine = strtok(clientInput,"\r\n");

  *errorCode = assignError(handleStartLine(request,startingLine), *errorCode);

  headerLine = strtok(NULL,"\r\n");
  while(headerLine||headerLine<startingLine){
    *errorCode = assignError(handleHeader(request,headerLine), *errorCode);
  }
  
  payload = doubleBreak+4;
  request->payload = payload;

  return request;
}

HttpReply* formReply(HttpRequest request, int* errorCode){
  //voodoo
}

char* replyToString(HttpReply reply){
    //strcat jumbo
}

int handleStartLine(HttpRequest* request, char* startingLine){
  //...
  return NO_ERROR;
}

int handleHeader(HttpRequest* request, char* startHeader){
  //...
  return NO_ERROR;
}


int assignError(int ret, int error){
  if(ret>error)return ret;
  return error;
}

