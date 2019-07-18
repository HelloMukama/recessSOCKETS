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

	int num1, num2, choice, ans;

    // NUMBER 1
	S : n = write(newsockfd, "Enter number 1: ", strlen("Enter number 1"));  // Ask client for number 1
	if(n < 0){
		error("Error writing to socket");
	}

	// sizeof(int) below means we are parsing int as a stream
	read(newsockfd, &num1, sizeof(int));   // Read number 1
	printf("Client - Number 1 is %d :\n", num1); 

    // NUMBER 2
	int nn = write(newsockfd, "Enter number 2: ", strlen("Enter number 2"));  // Ask client for number 2
	if(nn < 0){
		error("Error writing to socket");
	}
	read(newsockfd, &num2, sizeof(int));   // Read number 2
	printf("Client - Number 2 is %d :\n", num2);  

    
    int nnn = write(newsockfd, "Enter your choice: \n1.Addition \n2.Subtraction \n3.Multiplicatin \n4.Division \nExit \n", strlen("Enter your choice: \n1.Addition \n2.Subtraction \n3.Multiplicatin \n4.Division \nExit \n"));   // Ask for choice

    if(nnn < 0){
    	error("Error writing to socket");
    }

    read(newsockfd, &choice, sizeof(int));         // Read choice
    printf("Client - Choice is %d :\n", choice);

    switch(choice){
    	case 1: ans = num1 + num2;
    	    break;
    	case 2: ans = num1 - num2;
    	    break;
    	case 3: ans = num1 * num2;
    	    break;
    	case 4: ans = num1 / num2;
    	    break;
    	case 5: goto Q;
    	    break;
    }

    write(newsockfd, &ans, sizeof(int));
    if(choice != 5){
    	goto S; 
    }

	Q : close(newsockfd);
	close(sockfd);

	return 0;

}

