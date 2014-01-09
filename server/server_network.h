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
#include "server_network.h"

extern int status, sockd;
extern struct sockaddr_in my_name;
extern int server_handler;

int s_open_server(IncomingRequest *inc_request);
int s_close_server(IncomingRequest *inc_request);

#endif // SERVER_NETWORK_H_INCLUDED
