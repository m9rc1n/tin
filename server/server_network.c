// server
#include "server_network.h"

int s_open_server (int sockd, char* buffer, int buflen, struct sockaddr_in cli_name, int addrlen, int server_handler)
{
	FsOpenServerS open_server_s;
	FsOpenServerC* open_server_c;

    open_server_c = (FsOpenServerC*) buffer;

	open_server_s.server_handler = server_handler;
	open_server_s.answer = ACCEPTED;
	printf ("Opening session with: %d\n", open_server_s.server_handler);
	return sendto (sockd, (FsOpenServerS*) &open_server_s, sizeof (FsOpenServerS), 0, (struct sockaddr*) &cli_name, sizeof (cli_name));

}

int s_close_server (int sockd, char* buffer, int buflen, struct sockaddr_in cli_name, int addrlen, int server_handler)
{
	FsCloseServerS close_server_s;
	FsCloseServerC close_server_c;

    close_server_c = (FsCloseServerC*) buffer;

	printf ("Closing session with: %d\n", (*close_server_c).server_handler);
	close_server_s.answer = CLOSED;
	return sendto (sockd, (FsCloseServerS*) &close_server_s, sizeof (FsCloseServerS), 0, (struct sockaddr*) &cli_name, sizeof (cli_name));
}
