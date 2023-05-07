
//RUN SERVER.C TUTORIAL:
//========================================================================================
// To compile this program:
//      gcc server.c -o server 
//
// To run this program:
//      ./server <port>
//========================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//errror Function 
void error(const char *msg)
{
	
	perror(msg);
	exit(1);
	
}

int main(int argc,char *argv[])
{
	
	if(argc < 2)
	{
		
		fprintf(stderr,"Port Number not provided : Program Terminated \n");
		exit(1);
	}
	
	int sockfd;
	int newsockfd;
	int portno,n;
	char buffer[255];
	
	struct sockaddr_in server_addr,cli_addr;
	socklen_t clilen;
	
	sockfd = socket(AF_INET, SOCK_STREAM , 0);
	if(sockfd < 0 )
	{
		
        error("Error opening socket");
    }
	
	bzero((char *) &server_addr ,sizeof(server_addr));
	portno= atol(argv[1]);
	
	server_addr.sin_family= AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portno);
	
	if(bind(sockfd, (struct sockaddr *) &server_addr ,sizeof(server_addr)) < 0)
	{
		
	    error("Binding failed");
	}
	
	listen(sockfd,5);
	
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd ,(struct sockaddr *) &cli_addr , &clilen);
	
	if(newsockfd < 0)
	{
		
	    error("Errror on accept");
	}
	
	FILE *fp;
	int ch = 0;
	fp = fopen("FileReceived.txt", "a"); //if file exists then data will be appended, else it will be made
	int words;
	
	read(newsockfd, &words, sizeof(int));
	
	while(ch != words)
	{
		
		read(newsockfd, buffer, 255);
		fprintf(fp, "%s ", buffer);
		ch++;
	}
	
	printf("\n\n\n\t\t\t\tFile has been received successfully. It is saved by the name FileReceived.txt\n");
	
	close(newsockfd);
	close(sockfd);
	return 0;
	
}
