#include "tin_library.h"

/**
 * UDP jest protokolem bezpolaczeniowym,
 * wiec ta funkcja sluzy nam
 * do zarejestrowania klienta w serwerze
 *
 * wysylamy login a serwer jesli odpowie dobrze to zwracamy uchwyt
 */

#define PORT 44443

int sockd;
int port;
struct sockaddr_in my_addr;
struct sockaddr_in srv_addr;

int fs_open_server (char* server_address)
{
    FsResponse response;
    FsRequest request;
	FsOpenServerC open_server = {};
    request.command = OPEN_SERVER;
	request.request_data.open_server = open_server;

	socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sockd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockd == -1)
    {
        perror("Socket creation error");
        exit(1);
    }

	// client address
	my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = 0;
    status = bind (sockd, (struct sockaddr*)&my_addr, sizeof(my_addr));

  	// server address
	srv_addr.sin_family = AF_INET;
	inet_aton ("localhost", &srv_addr.sin_addr);

	printf ("input port: ");
	scanf ("%d", &port);
    srv_addr.sin_port = htons(port);

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.response_data.open_server.server_handler;
}

int fs_close_server (int server_handler)
{
    FsResponse response;
    FsRequest request;
    request.command = CLOSE_SERVER;
	FsCloseServerC close_server = {server_handler};
    request.request_data.close_server = close_server;

	socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
    printf("Sent close message\n");
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);
	printf ("Closing server: %d\n", response.answer);

    close (sockd);
	return 0;
}

int fs_open (int server_handler, char* name, int flags)
{
    return -1;
}

int fs_write (int server_handler, int fd, void *buf, size_t len)
{
    return -1;
}

int fs_read (int server_handler, int fd, long offset, int whence)
{
    return -1;
}

int fs_close (int server_handler, int fd)
{
    return -1;
}

int fs_lock (int server_handler, int fd, int mode)
{
    return -1;
}
