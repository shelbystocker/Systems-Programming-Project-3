#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "csapp.h"
#include "sserver.h"

#define MAX_VAL 8

int main(int argc, char **argv) {
	// check that user inputted correct number of arguments
        if (checkParams("run",argc) == 0) {
                fprintf(stderr, "Usage: %s host port secret value\n", argv[0]);
                exit(1);
        }

        // set arguments to variables
        char* machineName = argv[1];
        int port = atoi(argv[2]);
        int secretKey = atoi(argv[3]);
        char* data = argv[4];
	int dataLength = strlen(data);
        char* result = "";
        int* resultLength = 0;

        //if dataLength > 100, shorten value
        if (dataLength > MAX_VAL) {
                char tempVar[MAX_VAL];
                strncpy(tempVar, data, MAX_VAL);
                tempVar[MAX_VAL] = '\0';
                dataLength = MAX_VAL;
                data = tempVar;
        }

        // validate port and key
        if (port == 0 || secretKey == 0) {
                fprintf(stderr, "%s: port and secret cannot be 0\n", argv[0]);
                exit(1);
        }

        //call smallDigest in sserver.c
        smallRun(machineName, port, secretKey, data, result, resultLength);
	exit(0);

} // client


