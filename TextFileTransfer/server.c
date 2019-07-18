/*
NOTE that for every read on the server side, there must be a corresponding write on the client side.
And for every write on the server side, there must be a corresponding read on the client side

In most programs the skeleton of the code will always remain the same.
i.e from 

#include <stdio.h>
	.
	.
	.
	to
	.
	.
	
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
			error("Connection failed!");
		}

	.
	.
	.
	following here is the code that will suit the program you'll be developing

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // for writing our files as read, write and close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>      // defines the hostent structure

void error(const char *msg){
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[]){
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;      // hostent structure stores info about a given host e.g hostname and IPV4 address

	char buffer[255];

	if(argc < 3){  // if user has not provided the hostname (ip address) or port no
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){
		error("Error opening socket");
	}

	server = gethostbyname(argv[1]);     // the ip address of the server

	if(server == NULL){
		fprintf(stderr, "No such host!\n");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;

    // bcopy() below copies n bytes from x to y
	bcopy((char *) server->h_addr, (char *) & serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		error("Connection failed!");
	}

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0){
		error("Error on accepting the Connection");
	}

	FILE *fp;

	int ch = 0;
	fp = fopen("fooReceived.txt", "a");

	int words;

	read(newsockfd, &words, sizeof(int));

	while(ch != words){
		read(newsockfd, buffer, 255);
		fprintf(fp, "%s", buffer);
		ch++;
	}

	printf("The file has been received successfully as fooReceived.txt\n");


    close(newsockfd);  
    close(sockfd);  

	return 0;
}