// SSERVER.C 
// this is the source code for library sserver
// this is the actual smallSet and smallGet
// smallDigest and smallRun functions 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "csapp.h"
#include "sserver.h"

//---------FUNCTION CHECK_PARAMS--------
int checkParams(char* task, int argc) 
{
	int correctNumParams;
	if (strcmp(task,"set") == 0)
		correctNumParams = SET_PARAM_NUM;
	else if (strcmp(task,"get") == 0)
		correctNumParams = GET_PARAM_NUM;
	else if (strcmp(task,"digest") == 0)
		correctNumParams = DIGEST_PARAM_NUM;
	else 
		correctNumParams = RUN_PARAM_NUM;
	
	if (argc == correctNumParams)
		return 1;
	else 
		return 0;
	return 0;
}

//----------FUNCTION SMALL_SET------------
int smallSet(char *MachineName, int port, int SecretKey, char *variableName, char *value, int dataLength)
{
	int toserverfd = Open_clientfd(MachineName, port);
	// send secret key
	unsigned int key = SecretKey;	
	Rio_writen(toserverfd, &key, 4);
	// read if key was right into status
	unsigned int status;
	Rio_readn(toserverfd, &status, 4);
	if (status == 0) {
		// send request type
		short type = SET_TYPE;
		Rio_writen(toserverfd, &type, 2);
		// send padding
		short padding = 0;
		Rio_writen(toserverfd, &padding, 2);
		// send variable name
		char varName[MAX_NAME];
		strncpy(varName, variableName, MAX_NAME);
		varName[MAX_NAME] = '\0';
		Rio_writen(toserverfd, &varName, MAX_NAME);
		// send value length
		short dataL = dataLength;
		Rio_writen(toserverfd, &dataL, 2);
		// send value
		char varValue[dataLength];
		strcpy(varValue, value);
		varValue[dataLength] = '\0';
		Rio_writen(toserverfd, &varValue, dataLength);
	}
	Close(toserverfd);
	return 0;
}


//---------FUNCTION SMALL_GET-------------
int smallGet(char *MachineName, int port, int SecretKey, char *variableName, char *value, int *resultLength)
{
	int toserverfd = Open_clientfd(MachineName, port);
        // send secret key 
        unsigned int key = SecretKey;   
        Rio_writen(toserverfd, &key, 4);
        // read if key was right into status
        unsigned int status;
        Rio_readn(toserverfd, &status, 4);
        if (status == 0) {
                // send request type
		short type = GET_TYPE;
                Rio_writen(toserverfd, &type, 2);
                // send padding 
                short padding = 0;
                Rio_writen(toserverfd, &padding, 2);
                // send variable name
                char varName[MAX_NAME];
                strcpy(varName, variableName);
		varName[MAX_NAME] = '\0';
                Rio_writen(toserverfd, &varName, MAX_NAME);
		// read resultLength
		short resultL;
		Rio_readn(toserverfd, &resultL, 2);
		// read result
		char result[resultL];
		Rio_readn(toserverfd, &result, resultL);
		result[resultL] = '\0';
		// print result
		printf("%s\n", result);		
        }
        Close(toserverfd);
	return 0;

}

//---------FUNCTION SMALL_DIGEST----------
int smallDigest(char *MachineName, int port, int SecretKey, char *data, int dataLength, char *result, int *resultLength)
{
        int toserverfd = Open_clientfd(MachineName, port);
        // send secret key 
        unsigned int key = SecretKey;
        Rio_writen(toserverfd, &key, 4);
        // read if key was right into status
        unsigned int status;
        Rio_readn(toserverfd, &status, 4);
        if (status == 0) {
                // send request type
		short type = DIGEST_TYPE;
                Rio_writen(toserverfd, &type, 2);
                // send padding 
                short padding = 0;
                Rio_writen(toserverfd, &padding, 2);
                // send dataLength
                short dataL = (short)dataLength;
		Rio_writen(toserverfd, &dataL, 2);
                // send data
		char varData[dataLength];
		strcpy(varData, data);
		varData[dataL] = '\0';
		Rio_writen(toserverfd, &varData, dataLength);
		// read resultLength
                short resultL;
                Rio_readn(toserverfd, &resultL, 2);
		// read result
                char varResult[resultL];
                strcpy(varResult, result);
                Rio_readn(toserverfd, &varResult, resultL);
		varResult[resultL] = '\0';
		// print the sha256sum
		printf("%s\n", varResult);
	}
        Close(toserverfd);
        return 0;
}

//---------FUNCTION SMALL_RUN-------------
int smallRun(char *MachineName, int port, int SecretKey, char *request, char *result, int *resultLength)
{
	int toserverfd = Open_clientfd(MachineName, port);
        // send secret key
        unsigned int key = SecretKey;
        Rio_writen(toserverfd, &key, 4);
        // read if key was right into status
        unsigned int status;
        Rio_readn(toserverfd, &status, 4);
        if (status == 0) {
                // send request type
                short type = RUN_TYPE;
                Rio_writen(toserverfd, &type, 2);
                // send padding 
                short padding = 0;
                Rio_writen(toserverfd, &padding, 2);
                // send data
                char varRequest[8];
                strcpy(varRequest, request);
                varRequest[8] = '\0';
                Rio_writen(toserverfd, &varRequest, 8);
                // read resultLength
                short resultL;
                Rio_readn(toserverfd, &resultL, 2);
                // read result
                char varResult[resultL];
                strcpy(varResult, result);
                Rio_readn(toserverfd, &varResult, resultL);
                varResult[resultL] = '\0';
                // print the output
                printf("%s\n", varResult);
	}
        Close(toserverfd);
        return 0;
}
