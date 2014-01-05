// server
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../protocol.h"

#include "server_io.h"
#include "server_network.h"
#include "server_reset.h"
#include "server_synchroniser.h"

#define MAX_BUF 256


int main(int argc, char* argv[])
{
	int addrlen = sizeof(struct sockaddr_in);
	int status, sockd;
	struct sockaddr_in my_name, cli_name;
    char buffer[MAX_BUF];
    char command_buf[4];

	// kazdy klient dostaje inny numer
	// robimy kolejke klientow?
	int server_handler = 0;

	FsCommand* command;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s port_number\n", argv[0]);
		exit(1);
	}

	// create a socket
	sockd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockd == -1)
	{
		perror("Socket creation error");
		exit(1);
	}
	// server address
	my_name.sin_family = AF_INET;
	my_name.sin_addr.s_addr = INADDR_ANY;
	my_name.sin_port = htons (atoi (argv[1]));
	status = bind(sockd, (struct sockaddr*) &my_name, sizeof (my_name));

	/* Trzeba stworzyc i ruszyc synchronizator dostepu do plikow. */
	// fixme: error handling
	synchroniser_init();

	while (1)
	{
		recvfrom (sockd, (char*) &buffer, MAX_BUF, 0, (struct sockaddr*) &cli_name, &addrlen);
        command = (FsCommand*) &buffer;
		if (*command == OPEN_SERVER)
		{
			s_open_server(sockd, buffer, MAX_BUF, cli_name, addrlen, server_handler++);
		}
		else if (*command == CLOSE_SERVER)
		{
			s_close_server(sockd, buffer, MAX_BUF, cli_name, addrlen, server_handler);
			// @todo tymczasowe
			// zamykamy server, trzeba zrobic bardziej praktycznie, macie jakis pomysl?
			// zamykanie serwera przez klienta raczej nie wchodzi w gre, chyba ze jakis klient sterujacy serwerem
			// break;
		}
	}
	close(sockd);
	
	synchroniser_shutdown();
	return 0;
}
