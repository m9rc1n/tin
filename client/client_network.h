// client
#ifndef CLIENT_NETWORK_H_INCLUDED
#define CLIENT_NETWORK_H_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../protocol.h"

int server_handler = 0;

int c_open_server (void);
int c_close_server (void);

#endif // CLIENT_NETWORK_H_INCLUDED
