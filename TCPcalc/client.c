/*
NOTE that for every read on the server side, there must be a corresponding write on the client side.
And for every write on the server side, there must be a corresponding read on the client side




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

	int num1, num2, choice, ans;

    // NUMBER 1
	S : bzero(buffer, 255);
	// read() below is for the corresponding write() in the server.c file 
	n = read(sockfd, buffer, 255);     // Read server string

	if(n < 0){
		error("Error reading from socket");
	}

	printf("Server - %s\n", buffer);
	scanf("%d", &num1);                  // Enter num1
	write(sockfd, &num1, sizeof(int));   // Send num1 to server

    // NUMBER 2
	bzero(buffer, 255);
	// read() below is for the corresponding write() in the server.c file 
	int nn = read(sockfd, buffer, 255);     // Read server string

	if(nn < 0){
		error("Error reading from socket");
	}

	printf("Server - %s\n", buffer);
	scanf("%d", &num2);                  // Enter num2
	write(sockfd, &num2, sizeof(int));   // Send num2 to server

	// AT THIS POINT, we've already given num1 and num2 to the server. 
	// We are required to now give a choice to the server. 
	// i.e what type of manipulation should be done on the num1 and num2 that we sent to the server. 
	// e.g addition or division e.t.c

	bzero(buffer, 255);
	// read() below is for the corresponding write() in the server.c file 
	int nnn = read(sockfd, buffer, 255);     // Read server string

	if(nnn < 0){
		error("Error reading from socket");
	}

	printf("Server - %s\n", buffer);
	scanf("%d", &choice);                  // Enter choice
	write(sockfd, &choice, sizeof(int));   // Send choice to server

	if(choice == 5){
		goto Q;
	}

	read(sockfd, &ans, sizeof(int));
	printf("Server - The answer is %d \n", ans);

	if(choice != 5){
		goto S;
	}

	Q : 
	printf("Successfully exited the application!\n");
	close(sockfd);  

	return 0;
}