// SMALL_SET_C
/* this function is where you get all the variables to call 
smallSet() in the sserver.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "csapp.h"
#include "sserver.h"

int main(int argc, char** argv) {
	// check that user inputted correct number of arguments
	if (checkParams("set",argc) == 0) {
		fprintf(stderr, "Usage: %s host port secret variableName value\n", argv[0]);
		exit(1);
	}
	char* machineName = argv[1];
	int port = atoi(argv[2]);
	int secretKey = atoi(argv[3]);
	char* varName = argv[4];
	char* varValue = argv[5];
	int dataLength = strlen(varValue);
	int varLength = strlen(varName);

	//if variableName > 15, shorten it
	if (varLength > MAX_NAME) {
		char tempVar[MAX_NAME];
		strncpy(tempVar, varName, MAX_NAME);
		tempVar[MAX_NAME] = '\0';
		varName = tempVar;
	}

	//if dataLength > 100, shorten value
	if (dataLength > MAX_VALUE) {
		char tempVar[MAX_VALUE];
		strncpy(tempVar, varValue, MAX_VALUE);
		tempVar[MAX_VALUE] = '\0';
		dataLength = MAX_VALUE;
		varValue = tempVar;
	}	
	
	// checks that port and secret key are valid inputs
	if (port == 0 || secretKey == 0) {
		fprintf(stderr, "%s: port and secret cannot be 0\n", argv[0]);
		exit(1);
	}
	// calls smallSet function in sserver.c
	smallSet(machineName, port, secretKey, varName, varValue, dataLength);			
	return 0;
}

 

