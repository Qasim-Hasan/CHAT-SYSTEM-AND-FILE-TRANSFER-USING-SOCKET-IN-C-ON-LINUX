#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("usage %s hostname port\n", argv[0]);
        exit(1);
    }
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }
   
    int num1,num2,choice,ans;
    S: bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
    {
        error("Error reading from socket");
    }
    printf("Server: %s\n",buffer);
    scanf("%d",&num1);
    write(sockfd,&num1,sizeof(int));

    bzero(buffer,256);
    n= read(sockfd,buffer,255);
    if(n<0)
    {
        error("Error reading from socket");
    }
    printf("Server: %s\n",buffer);
    scanf("%d",&num2);
    write(sockfd,&num2,sizeof(int));

    bzero(buffer,256);
    n= read(sockfd,buffer,255);
    if(n<0)
    {
        error("Error reading from socket");
    }
    printf("Server: %s\n",buffer);
    scanf("%d",&choice);
    write(sockfd,&choice,sizeof(int));
    if(choice==5)
    {
        goto Q;
    }
    read(sockfd,&ans,sizeof(int));
    printf("Server: The answer is %d\n",ans);
    if(choice != 5)
    {
        goto S;
    }
    Q :
    printf("You have selected to exit.\nExit SUCCESSFUL\n");
    close(sockfd);
    return 0;
}
