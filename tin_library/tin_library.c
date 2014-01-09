#include "tin_library.h"

/**
 * UDP jest protokolem bezpolaczeniowym,
 * wiec ta funkcja sluzy nam
 * do zarejestrowania klienta w serwerze
 *
 * wysylamy login a serwer jesli odpowie dobrze to zwracamy uchwyt
 */

#define PORT 2000

int sockd;
int port;
struct sockaddr_in my_addr;
struct sockaddr_in srv_addr;

int fs_open_server (char* server_address)
{
    FsResponse response;
    FsRequest request;
    request.command = OPEN_SERVER;

    // size_t address_len = sizeof (request.request_data.open_server.server_address);
	// strncpy (request.request_data.open_server.server_address, server_address, address_len);

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
	inet_aton (server_address, &srv_addr.sin_addr);

	//printf ("input port: ");
	//scanf ("%d", &port);
    port = PORT;
    srv_addr.sin_port = htons(port);

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

    return response.response_data.open_server.server_handler;
}

int fs_close_server (int server_handler)
{
    printf("\t\t%d\n", server_handler);

    FsResponse response;

    FsRequest request;
    request.command = CLOSE_SERVER;
    request.request_data.close_server.server_handler = server_handler;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
    //printf("Sent close message, sid = %d\n", server_handler);
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);
	//printf ("Closing server: %d\n", response.answer);

	return response.response_data.close_server.status;
}

int fs_open (int server_handler, char* name, int flags)
{
    FsResponse response;

    FsRequest request;
    request.command = OPEN;
    request.request_data.open.server_handler = server_handler;
    strcpy (request.request_data.open.name, name);
    request.request_data.open.flags = flags;

	socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.response_data.open.status;
}

int fs_write (int server_handler, int fd, void *buf, size_t len)
{
    FsResponse response;

    FsRequest request;
    request.command = WRITE;
    request.request_data.write.server_handler = server_handler;
    request.request_data.write.fd = fd;
    memcpy (request.request_data.write.buffer, buf, len);
    request.request_data.write.buffer_len = len;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.response_data.write.status;
}

int fs_read (int server_handler, int fd, void *buf, size_t len)
{
    FsResponse response;

    FsRequest request;
    request.command = READ;
    request.request_data.read.server_handler = server_handler;
    request.request_data.read.fd = fd;
    memcpy (request.request_data.read.buffer, buf, len);
    request.request_data.read.buffer_len = len;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.response_data.read.status;
}

int fs_lseek (int server_handler, int fd, long offset, int whence)
{
    FsResponse response;

    FsRequest request;
    request.command = LSEEK;
    request.request_data.lseek.server_handler = server_handler;
    request.request_data.lseek.fd = fd;
    request.request_data.lseek.offset = offset;
    request.request_data.lseek.whence = whence;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.response_data.lseek.status;
}

int fs_close (int server_handler, int fd)
{
    FsResponse response;

    FsRequest request;
    request.command = CLOSE;
    request.request_data.close.server_handler = server_handler;
    request.request_data.close.fd = fd;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.response_data.close.status;
}

int fs_lock (int server_handler, int fd, int mode)
{
    FsResponse response;

    FsRequest request;
    request.command = LOCK;
    request.request_data.lock.server_handler = server_handler;
    request.request_data.lock.fd = fd;
    request.request_data.lock.mode = mode;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.response_data.lock.status;
}

int fs_fstat (int server_handler, int fd, FsFstat *buf)
{
    FsResponse response;

    FsRequest request;
    request.command = FSTAT;
    request.request_data.fstat.server_handler = server_handler;
    request.request_data.fstat.fd = fd;
    memcpy (&request.request_data.fstat.stat, buf, sizeof (FsFstat));

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.response_data.fstat.status;
}


