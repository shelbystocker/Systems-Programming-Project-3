#ifndef SSERVER_H_
#define SSERVER_H_

//define number of parameters
#define SET_PARAM_NUM 6
#define GET_PARAM_NUM 5
#define DIGEST_PARAM_NUM 5
#define RUN_PARAM_NUM 5

//define types
#define SET_TYPE 0
#define GET_TYPE 1
#define DIGEST_TYPE 2
#define RUN_TYPE 3

//define max values
#define MAX_NAME 15
#define MAX_VALUE 100

int checkParams(char* task, int argc);
int smallSet(char *MachineName, int port, int SecretKey, char *variableName, char *value, int dataLength);
int smallGet(char *MachineName, int port, int SecretKey, char *variableName, char *value, int *resultLength);
int smallDigest(char *MachineName, int port, int SecretKey, char *data, int dataLength, char *result, int *resultLength);
int smallRun(char *MachineName, int port, int SecretKey, char *request, char *result, int *resultLength);

#endif
