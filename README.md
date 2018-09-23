# Systems-Programming-Project-3
Folder: prog5.tgz
Included Files: smalld.c (server); smallSet.c, smallGet.c, smallDigest.c, smallRun.c (clients);
		sserver.c, sserver.h (client library); csapp.c, csapp.h (provided library).
		Makefile (makefile); README.txt (this file).
Authors: Jen Lee & Shelby Stocker
Project: Project 5, CS270-001
To Compile: "make"

This project creates a server with four client applications.
	The server will print out the secret key, request type, details, and completion
		status of each client call.
	How to run server: ./smalld port secretKey &
		Note: none of the clients will work if the port and secret key do not match
			the port and secret key specified in the command line of the server call.
			
The clients do as follows:

	- smallSet takes a variable name and a value. 
		The server sets that variable (internally to the server) to that value. 
		The client prints nothing unless the command fails, in which case it prints failed.
		How to run: ./smallSet MachineName port secretKey variable value
		
	- smallGet takes a variable name as its detail. 
		The server reports the contents of that variable or failure if such a variable is not set. 
		The client prints the contents it receives from the server, or failed.
		How to run: ./smallGet MachineName port secretKey variable
		
	- smallDigest returns the result of this invocation: /bin/echo value | /usr/bin/sha256sum . 
		The client prints the result it receives from the server, or failed.
		How to run: ./smallDigest MachineName port secretKey value
	- smallRun uses a value to specify a program. 
		The server runs that program and sends the first 100 bytes of its output back to the client. 
		Only certain programs are valid requests; the others must be rejected. 
		The valid values are:
			value	run this command
			inet	/sbin/ifconfig -a
			hosts	/bin/cat /etc/hosts
			service	/bin/cat /etc/services
		The client prints the result it receives from the server, or failed.
		How to run: ./smallRun MachineName port secretKey value

How to terminate program: pkill -e smalld
