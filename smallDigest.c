#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "csapp.h"
#include "sserver.h"
#define BUFFSIZE 1024

int main(int argc, char **argv) {
	// check that user inputted correct number of arguments
        if (checkParams("digest",argc) == 0) {
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
        if (dataLength > MAX_VALUE) {
                char tempVar[MAX_VALUE];
                strncpy(tempVar, data, MAX_VALUE);
                tempVar[MAX_VALUE] = '\0';
                dataLength = MAX_VALUE;
                data = tempVar;
        }

	// validate port and key
	if (port == 0 || secretKey == 0) {
                fprintf(stderr, "%s: port and secret cannot be 0\n", argv[0]);
                exit(1);
        }

	//call smallDigest in sserver.c
	smallDigest(machineName, port, secretKey, data, dataLength, result, resultLength);

      return 0;
}
 // client
