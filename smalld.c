// THIS IS THE SERVER
// File: smalld.c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "csapp.h"
#include "stdbool.h"

//Define const # of parameters
#define SERVER_PARAM_NUM 3
#define MAX_NAME 15
#define MAX_VALUE 100
#define BUFFSIZE 1024
// used to hold variables and their value
typedef struct {
	char Vvalue[MAX_VALUE];
	char Vname[MAX_NAME];
} variables;

variables var[BUFFSIZE];

int main(int argc, char** argv) {
	int listenfd, connfd, listenPort, secretKey;
	struct sockaddr_in clientAddr;
	unsigned int key;
	short type, padding;
	unsigned int status;
	
	// validate number of parameters
	if (argc != SERVER_PARAM_NUM) {
        	fprintf(stderr, "Need 3 arguments.\n");
        	exit(0);
	}

        listenPort = atoi(argv[1]); // gets the port
	secretKey = atoi(argv[2]); // gets the secret key

	// validates port and key
	if (listenPort == 0 || secretKey == 0) {
                fprintf(stderr, "%s: port and secret cannot be 0\n", argv[0]);
                exit(1);
        }

        listenfd = open_listenfd(listenPort); // open and return a listening socket on port
	socklen_t addrLength = sizeof(clientAddr);
	
	//counts the number of variables that have been made
	int count = 0;
	
	while (1) { // loop forever awaiting connection from a client
                connfd = Accept(listenfd, (SA *)&clientAddr, &addrLength); //Socket interface wrapper
		if (connfd < 1)
                        perror("accept error");
		
		// read in secret key
		Rio_readn(connfd, &key, 4);
		printf("Secret key = %d\n", key);
		
		// check if security key matches
                if (secretKey != key)
                        status = -1;
                else
                        status = 0;
                
		// write status of match
                Rio_writen(connfd, &status, 4);
		// read in request type
		Rio_readn(connfd, &type, 2);
		// read in bytes of padding
		Rio_readn(connfd, &padding, 2);
		
		//if secret key was correct and type = set	
		if (status == 0 && type == 0) {
			printf("Request type = set\n");
			//read in variable name
			Rio_readn(connfd, &var[count].Vname, MAX_NAME);
			//read in value length
			short dataLength;
			Rio_readn(connfd, &dataLength, 2);
			//read in value
			Rio_readn(connfd, &var[count].Vvalue, dataLength);
			printf("Detail = %s: %s\n", var[count].Vname, var[count].Vvalue);
			printf("Operation Status = success\n");
			printf("--------------------\n");
			count++;
		}
		
		//if secret key was correct and type = get
		if (status == 0 && type == 1) {
			printf("Request type = get\n");
			//read in variable name
			char varTemp[MAX_NAME];
			Rio_readn(connfd, &varTemp, MAX_NAME);
			printf("Detail = %s\n", varTemp);
			//loop to find variable name
			bool varFound = false;
			for (int i = count; i >= 0; i--) {
				if (strcmp(varTemp, var[i].Vname) == 0) {
					varFound = true;
					printf("Operation Status = success\n");
					printf("--------------------\n");
					short resultLength = (short)strlen(var[i].Vvalue);
					// send client result length
					Rio_writen(connfd, &resultLength, 2);
					// send client result value
					Rio_writen(connfd, &var[i].Vvalue, resultLength);
					break;					
				}
			}
			if (varFound == false) { //variable not found
				printf("Operation Status = failure\n");
				printf("--------------------\n");
				char fail[6];
				strcpy(fail, "failed");
				// send client fail length
				short resultLength = (short)strlen(fail);
				Rio_writen(connfd, &resultLength, 2);
				// send client failed
				Rio_writen(connfd, &fail, resultLength);
			}
		}
		
		//if secret key was correct and type = digest				
		if (status == 0 && type == 2) {
                        printf("Request type = digest\n");
                        // read in dataLength
                        short dataLength;
                        Rio_readn(connfd, &dataLength, 2);
                        // read in data
                        char data[dataLength];
                        Rio_readn(connfd, &data, dataLength);
			data[dataLength] = '\0';
			// clear the contents of /tmp/file.txt
		        fclose(fopen("/tmp/file.txt","w"));
        		char command[MAX_VALUE]; //store the command to run
        		sprintf(command, "/bin/echo %s | /usr/bin/sha256sum >> /tmp/file.txt", data);
        		system(command);
        		// read contents of /tmp/file.txt into buffer
        		//**********CITATION**********
			//URL: http://www.fundza.com/c4serious/fileIO_reading_all/
			FILE *infile;
        		char* buffer;
        		short resultLength;
        		infile = fopen("/tmp/file.txt", "r");
        		fseek(infile, 0L, SEEK_END);
        		resultLength = ftell(infile);
        		fseek(infile, 0L,SEEK_SET);
        		buffer = (char*)calloc(resultLength, sizeof(char));
        		fread(buffer, sizeof(char), resultLength, infile);
        		fclose(infile);
			// send resultLength
			Rio_writen(connfd, &resultLength, 2);
			// send result
			char result[resultLength];
			strcpy(result, buffer);
			result[resultLength] = '\0';
			Rio_writen(connfd, &result, resultLength);
                        // print statements
			printf("Detail = %s\n", data);
                        printf("Operation Status = success\n");
                        printf("--------------------\n");
                }
		//if secret key was correct and type = run
		if (status == 0 && type == 3) {
			printf("Request type = run\n");
			bool work = false;
			char command[MAX_VALUE]; //store the command to run
                        // read in value
			int dataLength = 8;
                        char value[dataLength];
                        Rio_readn(connfd, &value, dataLength);
                        value[dataLength] = '\0';
			printf("Detail = %s\n", value);
                        // clear the contents of /tmp/file.txt
                        fclose(fopen("/tmp/file.txt","w"));
                        //char command[MAX_VALUE]; //store the command to run
			if (strcmp(value, "inet") == 0) {
				strcpy(command, "/sbin/ifconfig -a");
				work = true;
			}
			else if (strcmp(value, "hosts") == 0) {
				strcpy(command, "/bin/cat /etc/hosts");
				work = true;
			}
			else if (strcmp(value, "service") == 0) {
				strcpy(command, "/bin/cat /etc/services");
				work = true;
			}
			if (work == false) {
				printf("Operation Status = failure\n");
                                printf("--------------------\n");
                                char fail[6];
                                strcpy(fail, "failed");
                                // send client fail length
                                short resultLength = (short)strlen(fail);
                                Rio_writen(connfd, &resultLength, 2);
                                // send client failed
                                Rio_writen(connfd, &fail, resultLength);
                        }
			else {
				printf("Operation Status = success\n");
				printf("--------------------\n"); 
				system(command);
                        	// read contents of /tmp/file.txt into buffer
                        	FILE *infile;
                        	char* buffer;
                        	short resultL;
                        	infile = fopen("/tmp/file.txt", "r");
                        	fseek(infile, 0L, MAX_VALUE);
                        	resultL = ftell(infile);
                        	fseek(infile, 0L,SEEK_SET);
				if (resultL >= 100)
                        	        resultL = 100;
				buffer = (char*)calloc(resultL, sizeof(char));
                        	fread(buffer, sizeof(char), resultL, infile);
                        	fclose(infile);
                        	Rio_writen(connfd, &resultL, 2);
                        	// send result
                        	char result[MAX_VALUE];
                        	strncpy(result, buffer,100);
                        	result[MAX_VALUE] = '\0';
                        	Rio_writen(connfd, &result, resultL);
			}
		}
                Close(connfd); //Unix I/O routine
     	 // loop forever
	}
	return 0;
} // server
