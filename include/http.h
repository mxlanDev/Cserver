#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define PROCESS_ERROR NULL
#define PATH_MAX 4096
#define BUFFER_SIZE 1024

enum httpMethod{
  GET,
  HEAD,
  POST,
  PUT,
  PATCH,
  DELETE,
  CONNECT,
  OPTIONS,
};

enum httpVersion{
  HTTP_10,
  HTTP_11,
  HTTP_20,
  HTTP_30,
};

enum httpStatus{
    Continue=100,
    Switching_Protocols=101,
    Created=201,
    OK=200,
    Accepted=202,
    Non_Authoritative_Information=203,
    No_Content=204,
    Reset_Content=205,
    Partial_Content=206,
    Multiple_Choices=300,
    Moved_Permanently=301,
    Found=302,
    See_Other=303,
    Not_Modified=304,
    Use_Proxy=305,
    Unused=306,
    Temporary_Redirect=307,
    Bad_Request=400,
    Unauthorized=401,
    Payment_Required=402,
    Forbidden=403,
    Not_Found=404,
    Method_Not_Allowed=405,
    Not_Acceptable=406,
    Proxy_Authentication_Required=407,
    Request_Timeout=408,
    Conflict=409,
    Gone=410,
    Length_Required=411,
    Precondition_Failed=412,
    Request_Entry_Too_Large=413,
    Request_URI_Too_Long=414,
    Unsupported_Media_Type=415,
    Request_Range_Not_Satisfiable=416,
    Expectations_Failed=417,
    Internal_Server_Error=500,
    Not_Implemented=501,
    Bad_Gateway=502,
    Service_Unavailable=503,
    Gateway_Timeout=504,
    HTTP_Version_Not_Supported=505,
};


typedef struct{
  char* key;
  char* value;
}HttpHeader;

typedef struct{
  enum httpMethod method;
  char* uri;
  enum httpVersion version;
  HttpHeader* headers;
  int headerCount;
  char* payload;
}HttpRequest;

typedef struct{
  enum httpVersion version;
  int httpStatus;
  HttpHeader* headers;
  int headerCount;
  char* actualPath;
}HttpReply;

HttpRequest* requestInit();
HttpReply* replyInit();

void requestDelete(HttpRequest* request);
void replyDelete(HttpReply* reply);

int stringToType(char* req);
char* typeToString(enum httpMethod type);
int stringToVersion(char* req);
char* versionToString(enum httpVersion type);
char* codeToMessage(enum httpStatus code);
char* statusToErrorPage(HttpReply* reply);

HttpRequest* loadRequest(char* clientInput, enum httpStatus* errorCode);
HttpReply* formReply(HttpRequest* request, enum httpStatus* errorCode);
void sendReplyHeaders(HttpReply* reply, int fd);

int addRequestHeader(HttpRequest* request, char* key, char* value);
int addReplyHeader(HttpReply* reply, char* key, char* value);

int handleStartLine(HttpRequest* request, char* startingLine);
enum httpStatus handleHeader(HttpRequest* request, char* headerLine);
int handlePayload(HttpRequest* request, char* payloadLine);

void addGeneralHeaders(HttpReply* reply);
void addEntityHeaders(HttpReply* reply,FILE* fptr);
void processGet(HttpReply* reply, HttpRequest* request);
void processHead(HttpReply* reply, HttpRequest* request);

enum httpStatus assignErrorStatus(enum httpStatus ret, enum httpStatus current);
