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

int fs_open_server (const char* server_address, int server_port)
{
    FsResponse response;
    FsRequest request;
    request.command = OPEN_SERVER;

    // @todo Potencjalnie do usuniecia w protokole, chyba ze podacie mi jakis powod dlaczego to
    // przekazac serwerowi
    // size_t address_len = sizeof (request.data.open_server.server_address);
	// strncpy (request.data.open_server.server_address, server_address, address_len);

	socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sockd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockd == -1)
    {
        perror("Socket creation error");
        exit(1);
    }

	/* client address */
	my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = 0;
    status = bind (sockd, (struct sockaddr*)&my_addr, sizeof(my_addr));

  	/* server address */
	srv_addr.sin_family = AF_INET;
	inet_aton (server_address, &srv_addr.sin_addr);

    port = server_port;
    srv_addr.sin_port = htons(port);

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

    printf ("Answer: %d", response.answer);

    return response.data.open_server.server_handler;
}

int fs_close_server (int server_handler)
{
    printf("\t\t%d\n", server_handler);

    FsResponse response;

    FsRequest request;
    request.command = CLOSE_SERVER;
    request.data.close_server.server_handler = server_handler;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

    sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
    //printf("Sent close message, sid = %d\n", server_handler);
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);
	//printf ("Closing server: %d\n", response.answer);

	return response.data.close_server.status;
}

int fs_open (int server_handler, const char* name, const char* mode)
{
    printf ("Dlugosc pliku w tescie: %lu\n", strlen(name));
    FsResponse response;

    FsRequest request;
    request.command = OPEN;
    request.data.open.server_handler = server_handler;
    request.data.open.name = (char*) malloc(strlen(name));
    strncpy (request.data.open.name, name, strlen(name));
    request.data.open.name_len = strlen(name);
    printf ("AQA: %s\n", request.data.open.name);
    /*request.data.open.mode = (char*) calloc(strlen(mode), sizeof(char));
    strncpy (request.data.open.mode, mode, strlen(mode));
    request.data.open.mode_len = strlen(mode);
*/
	socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	// recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

    free (request.data.open.name);
    free (request.data.open.mode);
	return response.data.open.status;
}

int fs_write (int server_handler, int fd, void *buf, size_t len)
{
    FsResponse response;

    FsRequest request;
    request.command = WRITE;
    request.data.write.server_handler = server_handler;
    request.data.write.fd = fd;
    request.data.write.buffer = (void*) malloc(len);
    memcpy (request.data.write.buffer, buf, len);
    request.data.write.buffer_len = len;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

    free (request.data.write.buffer);
	return response.data.write.status;
}

int fs_read (int server_handler, int fd, void *buf, size_t len)
{
    FsResponse response;

    FsRequest request;
    request.command = READ;
    request.data.read.server_handler = server_handler;
    request.data.read.fd = fd;
    request.data.read.buffer = (void*) malloc(len);
    memcpy (request.data.read.buffer, buf, len);
    request.data.read.buffer_len = len;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

    free (request.data.read.buffer);
	return response.data.read.status;
}

int fs_lseek (int server_handler, int fd, long offset, int whence)
{
    FsResponse response;

    FsRequest request;
    request.command = LSEEK;
    request.data.lseek.server_handler = server_handler;
    request.data.lseek.fd = fd;
    request.data.lseek.offset = offset;
    request.data.lseek.whence = whence;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.data.lseek.status;
}

int fs_close (int server_handler, int fd)
{
    FsResponse response;

    FsRequest request;
    request.command = CLOSE;
    request.data.close.server_handler = server_handler;
    request.data.close.fd = fd;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.data.close.status;
}

int fs_lock (int server_handler, int fd, int mode)
{
    FsResponse response;

    FsRequest request;
    request.command = LOCK;
    request.data.lock.server_handler = server_handler;
    request.data.lock.fd = fd;
    request.data.lock.mode = mode;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

	return response.data.lock.status;
}

int fs_fstat (int server_handler, int fd, struct stat* buf)
{
    FsResponse response;

    FsRequest request;
    request.command = FSTAT;
    request.data.fstat.server_handler = server_handler;
    request.data.fstat.fd = fd;
    request.data.fstat.stat = (FsFstat*) malloc(sizeof(FsFstat));
    request.data.fstat.stat->mode = buf->st_mode;
    request.data.fstat.stat->size = buf->st_size;
    request.data.fstat.stat->atime = buf->st_atime;
    request.data.fstat.stat->mtime = buf->st_mtime;
    request.data.fstat.stat->ctime = buf->st_ctime;

    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sendto (sockd, &request, sizeof(FsRequest), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	recvfrom(sockd, &response, sizeof(FsResponse), 0, (struct sockaddr*)&srv_addr, &addrlen);

    free (request.data.fstat.stat);
	return response.data.fstat.status;
}


