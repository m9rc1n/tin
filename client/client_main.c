// client
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "client_network.h"
#include "client_io.h"

int main(int argc, char* argv[])
{
	c_open_server();
	return 0;
}
