// client
#include "client_network.h"

int server_handler = 20;

int c_open_server (void)
{
	char* server_address;
	server_handler = fs_open_server ("0.0.0.0");
}

int c_close_server (void)
{
	fs_close_server (server_handler);
}

