#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // for writing our files as read, write and close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>      // defines the hostent structurev

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

	while(1){
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);

		// write below is write(socket file descriptor, what you are writing, lenght of what you are writing)
		n = write(sockfd, buffer, strlen(buffer));  // write takes 3 arguments. 3rd is length of buffer
		// 3rd param above means length of whatever the client will be sending to the server

		if(n < 0){
			error("Error on writing to the server");
		}
		bzero(buffer, 255);

		int nn = read(sockfd, buffer, 255);
		if(nn < 0){
			error("Error on reading from the server");
		}

		printf("Server SAYS: %s\n", buffer); 

		// next is the code for getting out of this loop

		// strncmp() below is a string comparison fn()
		int i = strncmp("BYE", buffer, 3);

		if(i == 0){   // if you DETECT the word 'BYE' in the msgs, end the conversation
			break;
		}
	}  // End while loop

	close(sockfd);  

	return 0;
}