#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../protocol.h"

int open_socket (void);
int open_server (void);

#endif // NETWORK_H_INCLUDED
