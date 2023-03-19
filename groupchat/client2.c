#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>


//errror Function 
void error(const char *msg){
perror(msg);
exit(1);
}

int main(int argc,char *argv[]){
int sockfd;
int newsockfd;
int portno,n;
char buffer[255];
struct sockaddr_in server_addr;
struct hostent *server;
if(argc < 3) {
fprintf(stderr,"usage %s hostname port \n",argv[0]);
exit(1);
}

portno = atol(argv[2]);
sockfd = socket(AF_INET, SOCK_STREAM , 0);
if(sockfd < 0 ){
error("Error opening socket");
}

server = gethostbyname(argv[1]);
if(server==NULL){
fprintf(stderr, "Error , no such host is active");
}


bzero((char *) &server_addr, sizeof(server_addr));
server_addr.sin_family =AF_INET;
bcopy((char *) server->h_addr ,(char *) &server_addr.sin_addr.s_addr, server->h_length);
server_addr.sin_port= htons(portno);

if(connect(sockfd, (struct sockaddr *) &server_addr ,sizeof(server_addr))<0)
error("Connection Failed");

while(1){
bzero(buffer,255);
fgets(buffer, 255 ,stdin);
n = write(sockfd, buffer ,strlen(buffer));
   if(n < 0){
   error("Error on writing");
   }
   bzero(buffer,255);
   n=read(sockfd,buffer,255);
   if(n<0){
   error("Error on reading.");
   }
   printf("Server: %s",buffer);   
   
   int i = strncmp("Bye",buffer,3);
   if(i==0){
   break;
   }
   
}

close(sockfd);
return 0;

}
