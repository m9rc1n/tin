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

int s_open_server (int sockd, char* buffer, int buflen, struct sockaddr_in cli_name, int addrlen, int server_handler);
int s_close_server (int sockd, char* buffer, int buflen, struct sockaddr_in cli_name, int addrlen, int server_handler);

#endif // SERVER_NETWORK_H_INCLUDED
