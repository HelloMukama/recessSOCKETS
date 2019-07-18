#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>       // will enable the isspace() below to work

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

	bzero(buffer, 255);

	FILE *f;
	int words = 0;
	char c;

	f = fopen("foo.txt", "r"); // file pointer will go at start of foo.txt

	// next we are calculating the words in foo.txt
	while((c = getc(f)) != EOF){  // getc is getting the character pointed to by the file pointer
		fscanf(f, "%s", buffer);

		if(isspace(c) || c=='\t'){
			words++;
		}
	}

    // below, we are writing the word count from the above loop to the server
	write(sockfd, &words, sizeof(int));
	rewind(f);

	// NEXT, WE ARE WRITING TO THE SERVER WORD BY WORD THE CONTENTS OF foo.txt
	char ch;
	while(ch != EOF){
		fscanf(f, "%s", buffer);
		write(sockfd, buffer, 255);
		ch = fgetc(f);
	}

	printf("The file has been successfully sent. Thank you!");


	










	close(newsockfd);
	close(sockfd);

	return 0;

}

