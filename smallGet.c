// this is where you get all the parameters to call
// smallGet() in sserver.c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "csapp.h"
#include "sserver.h"

int main(int argc, char** argv) {
        // check that user inputted correct number of arguments
        if (checkParams("get",argc) == 0) {
                fprintf(stderr, "Usage: %s host port secret variableName\n", argv[0]);
                exit(1);
        }
	// read in values from arguments
        char* machineName = argv[1];
        int port = atoi(argv[2]);
        int secretKey = atoi(argv[3]);
        char* varName = argv[4];
        int* resultLength = 0;
	char *varValue = "";

	//if variableName > 15, shorten it
        int varLength = strlen(varName);
	if (varLength > MAX_NAME) {
                char tempVar[MAX_NAME];
                strncpy(tempVar, varName, MAX_NAME);
                tempVar[MAX_NAME] = '\0';
                varName = tempVar;
        }
	
	// validate port and key
        if (port == 0 || secretKey == 0) {
                fprintf(stderr, "%s: port and secret cannot be 0\n", argv[0]);
                exit(1);
        }

	// call smallGet() in sserver.
	smallGet(machineName, port, secretKey, varName, varValue, resultLength);
	return 0;
}

