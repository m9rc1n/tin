#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "network.h"
#include "io.h"

/**
 * To jest przykladowe ustawienie:
 * Normalnie klient tu powinny sie znajdowac funkcje interakcji uzytkownika z komputerem
 * Klient laczy sie z serwerem poprzez biblioteke tin_library
 */

int main(int argc, char* argv[])
{
	open_server();

	// fs
	/* create a socket */
	/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		dodałem to
		implementacja w bibliotece
	*/
	return 0;
}
