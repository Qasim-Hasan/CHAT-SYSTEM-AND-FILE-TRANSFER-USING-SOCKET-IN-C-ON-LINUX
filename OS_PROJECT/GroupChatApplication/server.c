
//========================================================================================
// To compile this program:
//      gcc groupchat_linux_server.c -D_REENTRANT -o server -lpthread
//                                   ------------           ---------
//                                        |                     |
//                 drives the compiler to use            adds support for multithreading
//                 thread safe (i.e., re-entrant)        with the pthreads library
//                 versions of several functions     
//                 in the C library  
//
// To run this program:
//      ./server <port>
//
//========================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MSG_LEN     2048
#define MAX_CLIENT  512

// function declarations
void* serve_cl(void *arg);
void broadcast_msg(char *msg, int len);
void handle_err(char *err_msg);

// global variables 
int cl_cnt = 0;             // counter for connected clients
int cl_socks[MAX_CLIENT];   // array (list) of the connected clients
pthread_mutex_t mutex;  


int main(int argc, char *argv[])
{
    int sv_sock, cl_sock;                   // server/client socket file descriptors
    struct sockaddr_in sv_addr, cl_addr;    // Internet socket address structures
    int cl_addr_sz;                         // size of the client sockaddr_in structure
    pthread_t t_id;                         // thread ID

    // handle invalid number of arguments
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);

    // create a TCP socket
    sv_sock = socket(PF_INET, SOCK_STREAM, 0);  

    // allocate and initialize Internet socket address structure (server info)
    // specify server's IP address and port
    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sv_addr.sin_port = htons(atoi(argv[1]));

    // bind server's address information to the server socket
    if (bind(sv_sock, (struct sockaddr*)&sv_addr, sizeof(sv_addr)) == -1)
        handle_err("ERROR: bind() fail");

    // convert the server socket to listening socket
    if (listen(sv_sock, 5) == -1)
        handle_err("ERROR: listen() fail");

    // clear the screen
    system("clear");

    // print the welcome message to the screen
    printf("Group chat linux server running ... (port: %s)\n", argv[1]);

    while (1)
    {
        // get the size of Internet socket address structure
        cl_addr_sz = sizeof(cl_addr);

        // accept client connection
        cl_sock = accept(sv_sock, (struct sockaddr*)&cl_addr, &cl_addr_sz);

        // add the new client to the list
        pthread_mutex_lock(&mutex);     // enter the critical section   
        cl_socks[cl_cnt++] = cl_sock;  
        pthread_mutex_unlock(&mutex);   // leave the critical section

        // create a thread to serve the client
        pthread_create(&t_id, NULL, serve_cl, (void*)&cl_sock); 

        // detach the thread from the main thread so that it automatically destroys itself
        // upton termination
        pthread_detach(t_id); 

        // print the connected client's IP address to the screen
        printf("Connected client IP: %s\n", inet_ntoa(cl_addr.sin_addr));
    }

    close(sv_sock);
   
    return 0;
} // end of main

// receive client's message, and broadcast it to all connected clients
void* serve_cl(void *arg)
{
    int cl_sock = *((int*)arg);
    int str_len = 0, i;
    char msg[MSG_LEN];

    // this while loop won't break until the client terminates the connection
    while ((str_len = read(cl_sock, msg, sizeof(msg))) != 0)
        broadcast_msg(msg, str_len);
    
    pthread_mutex_lock(&mutex);    // enter the critical section 

    // before terminating the thread, remove itself (disconnected client) from the list
    for (i = 0; i < cl_cnt; i++)    
    {
        if (cl_sock == cl_socks[i])
        {
            while (i < cl_cnt - 1)
            {
                cl_socks[i] = cl_socks[i + 1];
                i++;
            }

            break;
        }
    }
    
    cl_cnt--;                       // decrement the client counter
    pthread_mutex_unlock(&mutex);   // leave the critical section 

    // terminate the connection
    close(cl_sock);

    return NULL;
} // end of serve_cl

// broadcast message to all connected clients
void broadcast_msg(char *msg, int len)
{
    int i;

    pthread_mutex_lock(&mutex);     // enter the critical section 

    // send message to every connected clients
    for (i = 0; i < cl_cnt; i++)
        write(cl_socks[i], msg, len);

    pthread_mutex_unlock(&mutex);   // leave the critical section 
} // end of broadcast_msg

// handle error
void handle_err(char *err_msg)
{
    fputs(err_msg, stderr);
    fputc('\n', stderr);
    exit(1);
} // end of handle
