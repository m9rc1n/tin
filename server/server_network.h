// server
#ifndef SERVER_NETWORK_H_INCLUDED
#define SERVER_NETWORK_H_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../protocol.h"

#define MAX_BUF 256

struct IncomingRequest {
    struct sockaddr_in client_addr;
    unsigned int client_addr_len;
    
    FsRequest request;
};

extern int status, sockd;
extern struct sockaddr_in my_name;
extern int server_handler;

int s_open_server(int sockd, struct IncomingRequest *inc_request, int server_handler);
int s_close_server(int sockd, FsRequest *fs_request, struct sockaddr_in client_name, int addrlen, int server_handler);

#endif // SERVER_NETWORK_H_INCLUDED
