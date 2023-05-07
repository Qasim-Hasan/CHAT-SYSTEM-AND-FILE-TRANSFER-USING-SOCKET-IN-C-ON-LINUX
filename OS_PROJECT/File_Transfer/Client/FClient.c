
//RUN CLIENT.C TUTORIAL:
//========================================================================================
// To compile this program:
//      gcc client.c -o client 
//
// To run this program:
//      ./client <ip> <port>
//========================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>


//errror Function 
void error(const char *msg)
{
	
	perror(msg);
	exit(1);
}

int main(int argc,char *argv[])
{
	
	int sockfd;
	int newsockfd;
	int portno,n;
	char buffer[255];
	struct sockaddr_in server_addr;
	struct hostent *server;
	if(argc < 3)
	{
		
		fprintf(stderr,"usage %s hostname port \n",argv[0]);
		exit(1);
	}
	
	portno = atol(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM , 0);
	if(sockfd < 0 )
	{
		
	    error("Error opening socket");
	}
	
	server = gethostbyname(argv[1]);
	if(server==NULL)
	{
		
        fprintf(stderr, "Error , no such host is active");
	}
	
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family =AF_INET;
	bcopy((char *) server->h_addr ,(char *) &server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port= htons(portno);
	
	if(connect(sockfd, (struct sockaddr *) &server_addr ,sizeof(server_addr))<0)
	{
		
        error("Connection Failed");
	}
	
	bzero(buffer,255);
	
	FILE *f; // file pointer
	int words = 0; //for loop counting depending on number of words that should be sent
	
	char c;
	f = fopen("file.txt" , "r"); //opening in read only form
	while((c = getc(f))!= EOF)
	{
		
		fscanf(f , "%s", buffer);
		if(isspace(c) || c =='\t')
		{                           // isspace checks whether character is a space because if space
			
			words++;                //comes that means a word is completed.
		}
		
	}
	
	write(sockfd, &words, sizeof(int)); //printing number of words in server
	rewind(f);                           //move file pointer to the beginning
	
	char ch;
	while(ch != EOF)
	{                                   //writing on server
		fscanf(f, "%s", buffer);
		write(sockfd, buffer, 255);
		ch = fgetc(f);
		
	}
	
	printf("\n\n\t\t\t\tThe file hass been successfully sent.\n");
	
	close(sockfd);
	return 0;

}
