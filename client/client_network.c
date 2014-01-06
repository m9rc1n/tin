#include "client_network.h"
#include "../tin_library/tin_library.h"

int server_handler = 20;

int c_open_server(void)
{
	server_handler = fs_open_server("0.0.0.0");
    return 42;
}

int c_close_server(void)
{
	fs_close_server (server_handler);
    return 42;
}

