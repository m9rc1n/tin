// server
#ifndef SERVER_PROTOCOL_H_INCLUDED
#define SERVER_PROTOCOL_H_INCLUDED

#include "../protocol.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct IncomingRequest
{
    struct sockaddr_in client_addr;
    unsigned int client_addr_len;
    FsRequest request;
}
IncomingRequest;

#endif // SERVER_PROTOCOL_H_INCLUDED
