#include <string.h>

#define PROCESS_ERROR -1


#define SYNTAX_ERROR 10
#define PATH_ERROR 9
#define NO_ERROR 0



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

typedef struct{
  char* key;
  char** values;
}HttpHeader;

typedef struct{
  enum httpMethod method;
  char* uri;
  enum httpVersion version;
  HttpHeader* headers;
  char* payload;
}HttpRequest;

typedef struct{
  enum httpVersion version;
  int httpStatus;
  char* uri;
  HttpHeader* headers;
}HttpReply;

int stringToType(char* req);
char* typeToString(enum httpMethod type);
int stringToVersion(char* req);
char* versionToString(enum httpVersion type);
char* codeToMessage(unsigned int code);

HttpRequest* loadRequest(char* clientInput, int* errorCode);
HttpReply* formReply(HttpRequest request, int* errorCode);
char* replyToString(HttpReply reply);

int handleStartLine(HttpRequest* request, char* startingLine);
int handleHeader(HttpRequest* request, char* headerLine);
int handlePayload(HttpRequest* request, char* payloadLine);



int assignError(int ret, int current);
