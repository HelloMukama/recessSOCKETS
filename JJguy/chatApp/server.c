#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}

// argc is total number of parameters we are passing i.e two => 1=file_name and 2=port_number
int main(int argc, char *argv[]){    
	if(argc < 2){
		fprintf(stderr ,"Port number not provided. Program terminated!\n");
		exit(1);
	}

	int sockfd, newsockfd, portno, n;
	char buffer[255];

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;      // socklen_t is a datatype in socket.h and it is a 32bit datatype

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){
		error("Error opening socket.");
	}
	// bzero below clears any text or data it is referenced to
	bzero((char *) &serv_addr, sizeof(serv_addr));

	// BELOW, argv[] contains the original commandline arguments. 
	// argv[0] contains filename while argv[1] contains portno.
	portno = atoi(argv[1]);  // atoi converts a string into an integer

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	// BELOW, htons = host to network short  ALSO htonl = host to network long
	serv_addr.sin_port = htons(portno);   

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		error("Binding failed");
	}

	listen(sockfd, 5); // 5 here means the maximum limit of clients that can connect to  the server at a time
	clilen = sizeof(cli_addr);

	// at this point we are accepting the connection.
	// note that whenever a connection is accepted, we get a new file descriptor i.e 'newsockfd' below
	newsockfd =  accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);  
	// line above means....CONNECTION ACCEPTED. NEW FILE DESCRIPTOR GOTTEN

	// NEXT IS TO CHECK IF CONNECTION'S BEEN ACCEPTED ANYWAY.
	if(newsockfd < 0){    // 0 is success and -1 is failure
		error("Error on Connection acception");
	}

	while(1){
		
		// we'll be using buffer below to store a message and then send it over a network in form of a string
		bzero(buffer, 255);
		// read fn() below means there must be a corresponding write fn() in the client.c code
		n = read(newsockfd, buffer, 255);

		if(n < 0){
			error("Error on reading from client!");
		}
		printf("Incoming message: %s\n", buffer);   // printing message from client

		// time for the server to reply. FIRST, WE'LL NEED TO CLEAR THE BUFFER.
		bzero(buffer, 255);

		// fgets() below is from the stdio.h library and it reads bytes from streams. stdin below, 
		// is the input stream i.e the keyboard and fgets will get input and 
		// save it into the array pointed to by the buffer pointer since buffer is a character array
		fgets(buffer, 255, stdin);  

		// now, fgets() will get reply from the server and send it to the client with the write fn() below.
		int nn = write(newsockfd, buffer, strlen(buffer)); 

		if(nn < 0){
			error("Error on replying to the Client");
		} 

		// next is the code for getting out of this loop

		// strncmp() below is a string comparison fn()
		int i = strncmp("BYE", buffer, 3);

		if(i == 0){   // if you DETECT the word 'BYE' in the msgs, end the conversation
			break;
		}
	}

	close(sockfd);
	close(newsockfd);

	return 0;

}

