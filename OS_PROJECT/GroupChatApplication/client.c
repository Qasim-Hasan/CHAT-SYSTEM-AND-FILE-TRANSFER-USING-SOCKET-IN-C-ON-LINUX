// RUN CLIENT.C TUTORIAL:
//========================================================================================
// To compile this program:
//      gcc groupchat_linux_client.c -D_REENTRANT -o client -lpthread
//                                   ------------           ---------
//                                        |                     |
//                 drives the compiler to use            adds support for multithreading
//                 thread safe (i.e., re-entrant)        with the pthreads library
//                 versions of several functions     
//                 in the C library            
//
// To run this program:
//      ./client <serverIP> <port> <name>
//========================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MSG_LEN     1024
#define NAME_LEN    32

// function declarations
void* send_msg(void *arg);
void* recv_msg(void *arg);
void handle_err(char *err_msg);

// global variables 
char name[NAME_LEN] = "noname";
char msg[MSG_LEN];
char info_msg[MSG_LEN];


int main(int argc, char *argv[])
{
    int sock;                           // client socket file descriptor
    struct sockaddr_in sv_addr;         // Internet socket address structure
    pthread_t send_thread, recv_thread; // two threads of the client process
    void *thread_return;                // pointer to the thread's return value

    // handle invalid number of arguments
    if (argc != 4)
    {
        printf("Usage: %s <serverIP> <port> <name>\n", argv[0]);
        exit(1);
    }

    sprintf(name, "%s", argv[3]);  // setup user name

    // create a TCP socket
    sock = socket(PF_INET, SOCK_STREAM, 0); 

    // allocate and initialize the Internet socket address structure (server info)
    // specify server's IP address and port
    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sv_addr.sin_port = htons(atoi(argv[2]));

    // establish connection with server
    if (connect(sock, (struct sockaddr*)&sv_addr, sizeof(sv_addr)) == -1)
        handle_err("ERROR: connect() fail");

    // clear the screen
    system("clear");

    // print the welcome message to the screen
    printf("Welcome to group chat! (Q/q to quit)\n\n");

    // print the join message to the screen
    sprintf(msg, "+++++ %s has joined! +++++\n", name);
    write(sock, msg, strlen(msg));

    // create two threads to carry out send and receive operations, respectively
    pthread_create(&send_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&recv_thread, NULL, recv_msg, (void*)&sock);
    pthread_join(send_thread, &thread_return);
    pthread_join(recv_thread, &thread_return);

    // terminate connection
    close(sock);

    return 0;
} // end of main

// send message to the server
void* send_msg(void *arg)
{
    int sock = *((int*)arg);
    char name_msg[NAME_LEN + MSG_LEN + 2];  // add '2' for brackets around the name

    while (1)
    {
        fgets(msg, MSG_LEN, stdin);

        // terminate the connection upon user input Q/q
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
        {
            // print the leave message to the screen
            sprintf(msg, "----- %s has left! -----\n", name);
            write(sock, msg, strlen(msg));

            // terminate the connection
            close(sock);
            exit(0);
        }

        // construct the message and send it to the server
        sprintf(name_msg, "[%s] %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    
    return NULL;
} // end of send_msg

// receive message and print it to the screen
void* recv_msg(void *arg)
{
    int sock = *((int*)arg);
    char name_msg[NAME_LEN + MSG_LEN + 2];
    int str_len;

    while(1)
    {
        // receive the message broadcasted by the server
        str_len = read(sock, name_msg, NAME_LEN + MSG_LEN + 2 - 1); 
            // add '2' for brackets around the name

        // terminate the thread if the client has been disconnected
        if (str_len == -1)
            return (void*)-1;

        // print the received message to the screen
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }

    return NULL;
} // end of recv_msg

// handle error
void handle_err(char *err_msg)
{
    fputs(err_msg, stderr);
    fputc('\n', stderr);
    exit(1);
} // end of handle_err
