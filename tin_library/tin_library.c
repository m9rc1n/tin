#include "tin_library.h"
#include <errno.h>

/**
 * UDP jest protokolem bezpolaczeniowym,
 * wiec ta funkcja sluzy nam
 * do zarejestrowania klienta w serwerze
 *
 * wysylamy login a serwer jesli odpowie dobrze to zwracamy uchwyt
 */

#define PORT 2000
#define WAIT_TO_STOP_RCV 1
#define WAIT_TO_SEND 1
#define WAIT_TO_RCV 1

int sockd;
int port;
struct sockaddr_in my_addr;
struct sockaddr_in srv_addr;
int fs_open_server (const char* server_address, int server_port)
{
    FsResponse response;
    FsRequest request;
    request.command = OPEN_SERVER;
    socklen_t addrlen = sizeof(struct sockaddr_in);
	int status, count;

	sockd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockd == -1)
    {
        perror("Socket creation error");
        return -1;
    }

	/* client address */
	my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = 0;

    status = bind (sockd, (struct sockaddr*)&my_addr, sizeof(my_addr));

    if (status != 0)
    {
        close(sockd);
        sockd = -1;
        perror("Socket binding error");
        return -1;
    }

  	/* server address */
	srv_addr.sin_family = AF_INET;
	inet_aton (server_address, &srv_addr.sin_addr);
    srv_addr.sin_addr.s_addr = inet_addr(server_address);
    port = server_port;
    srv_addr.sin_port = htons(port);

    struct timeval time_val;

    time_val.tv_sec = WAIT_TO_STOP_RCV;
    time_val.tv_usec = 0;

    setsockopt(sockd, SOL_SOCKET, SO_RCVTIMEO, (char*) &time_val, sizeof(struct timeval));

    status = connect (sockd, (struct sockaddr*) &srv_addr, sizeof(srv_addr));

    status = send(sockd, &request, sizeof(FsRequest), 0);
    count = recv(sockd, &response, sizeof(FsResponse), 0);

    if (errno != 0) {
        perror("Receiving packets error");
        return -1;
    }

    info (response.answer);
    return response.data.open_server.server_handler;
}

int fs_close_server (int server_handler)
{
    FsResponse response;

    FsRequest request;
    request.command = CLOSE_SERVER;
    request.data.close_server.server_handler = server_handler;

	int status, count;

    if (sockd == -1)
    {
        perror("Socket is closed");
        return -1;
    }

    status = send(sockd, &request, sizeof(FsRequest), 0);
	count = recv(sockd, &response, sizeof(FsResponse), 0);

    if (errno != 0) {
        perror("Receiving packets error");
        return -1;
    }

    info (response.answer);
	return response.data.close_server.status;
}

int fs_open (int server_handler, const char* name, const char* mode)
{
    FsResponse response;

    FsRequest request;
    request.command = OPEN;
    request.data.open.server_handler = server_handler;
    strncpy (request.data.open.name, name, strlen(name));
    request.data.open.name_len = strlen(name);
    strncpy (request.data.open.mode, mode, strlen(mode));
    request.data.open.mode_len = strlen(mode);

    printf("\tX:%lu [%s]\n", request.data.open.mode_len, mode);

    int status, count;
    if (sockd == -1)
    {
        perror("Socket is closed");
        return -1;
    }

	status = send(sockd, &request, sizeof(FsRequest), 0);
	count = recv(sockd, &response, sizeof(FsResponse), 0);

    if (errno != 0) {
        perror("Receiving packets error");
        return -1;
    }

    info(response.answer);
	return response.data.open.fd;
}

int fs_write (int server_handler, int fd, const void *buf, size_t len)
{
 	int i=0, status=0;
    size_t count = 0;;
    size_t parts = (len-1)/BUF_LEN;
    size_t last_part = len%BUF_LEN;

    FsResponse response;

    FsRequest request;
    request.command = WRITE;
    request.data.write.server_handler = server_handler;
    request.data.write.fd = fd;
    request.data.write.buffer_len = len;
    request.data.write.parts_number = parts+1;

    if (sockd == -1)
    {
        perror("Socket is closed");
        return -1;
    }

    status = send (sockd, &request, sizeof(FsRequest), 0);
    count = recv (sockd, &response, sizeof(FsResponse), 0);

    if (info(response.answer) == -1) return response.data.write.status;

    if (response.answer == IF_CONTINUE)
    {
        request.command = WRITE_PACKAGES;
        for(i=0; i<parts; i++)
        {
            strncpy (request.data.write.buffer, buf + i * BUF_LEN, BUF_LEN);
            request.data.write.part_id = i;
            request.data.write.buffer_len = BUF_LEN;
            status = send (sockd, &request, sizeof(FsRequest), 0);
        }
        if (last_part == 0) last_part = BUF_LEN;
        request.data.write.buffer_len = last_part;
        strncpy (request.data.write.buffer, buf + i * BUF_LEN, last_part);
        request.data.write.part_id = i;
        status = send (sockd, &request, sizeof(FsRequest), 0);

        sleep(WAIT_TO_SEND);

        request.command = WRITE_ALL;
        status = send (sockd, &request, sizeof(FsRequest), 0);
        count = recv (sockd, &response, sizeof(FsResponse), 0);

        if (errno != 0)
        {
            perror("Error while receiving packets");
            return -1;
        }
    }
    info(response.answer);
    return response.data.write.status;
}

int fs_read (int server_handler, int fd, void *buf, size_t len)
{
    int status=0;
    size_t count = 0;
    size_t parts = (len-1)/BUF_LEN+1;
    size_t last_part = len%BUF_LEN;
    int* received_parts = (int*) calloc(parts, sizeof(int));
    FsResponse response;

    FsRequest request;
    request.command = READ;
    request.data.read.server_handler = server_handler;
    request.data.read.fd = fd;
    request.data.read.buffer_len = len;

	if (sockd == -1)
    {
        perror("Socket is closed");
        return -1;
    }

    status = send(sockd, &request, sizeof(FsRequest), 0);

    for(int i=0; i<parts; ++i)
    {
        count = recv(sockd, &response, sizeof(FsResponse), 0);
        strncpy (buf + response.data.read.part_id * BUF_LEN, response.data.read.buffer, response.data.read.buffer_len);
        int *current_index = received_parts + i;
        *current_index = 1;
    }

    sleep (WAIT_TO_RCV);
    count = recv(sockd, &response, sizeof(FsResponse), 0);

    if (errno != 0) {
        perror("Receiving packets error");
        return -1;
    }

    // check if correct create file
    for(int i=0; i<parts; ++i)
    {
        int* current_index = received_parts + i;
        if (*current_index == 0)
        {
            response.answer = EF_CORRUPT_PACKAGE;
            break;
        }
    }

    if (response.answer == IF_OK)
    {
        // FILE* new_file = fopen(response.answer.data.name, "w");
        FILE* new_file = fopen("a.x", "w");
        if (new_file == NULL)
        {
            perror("Cannot save file, which was read from server");
        }
        // fwrite(buf, sizeof(char), response.answer.data.read.file_size, new_file);
    }

    info(response.answer);
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

	int status, count;

    if (sockd == -1)
    {
        perror("Socket is closed");
        return -1;
    }

	status = send(sockd, &request, sizeof(FsRequest), 0);
	count = recv(sockd, &response, sizeof(FsResponse), 0);

    if (errno != 0) {
        perror("Receiving packets error");
        return -1;
    }

    info(response.answer);
	return response.data.lseek.status;
}

int fs_close (int server_handler, int fd)
{
    FsResponse response;

    FsRequest request;
    request.command = CLOSE;
    request.data.close.server_handler = server_handler;
    request.data.close.fd = fd;

	int status, count;

    if (sockd == -1)
    {
        perror("Socket is closed");
        return -1;
    }

	status = send(sockd, &request, sizeof(FsRequest), 0);
	count = recv(sockd, &response, sizeof(FsResponse), 0);

    if (errno != 0) {
        perror("Receiving packets error");
        return -1;
    }

    info(response.answer);
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

	int status, count;

    if (sockd == -1)
    {
        perror("Socket is closed");
        return -1;
    }

	status = send(sockd, &request, sizeof(FsRequest), 0);
	count = recv(sockd, &response, sizeof(FsResponse), 0);

    if (errno != 0) {
        perror("Receiving packets error");
        return -1;
    }

    info(response.answer);
	return response.data.lock.status;
}

int fs_fstat (int server_handler, int fd, struct stat* buf)
{
    FsResponse response;

    FsRequest request;
    request.command = FSTAT;
    request.data.fstat.server_handler = server_handler;
    request.data.fstat.fd = fd;
    request.data.fstat.stat.mode = buf->st_mode;
    request.data.fstat.stat.size = buf->st_size;
    request.data.fstat.stat.atime = buf->st_atime;
    request.data.fstat.stat.mtime = buf->st_mtime;
    request.data.fstat.stat.ctime = buf->st_ctime;

	int status, count;

    if (sockd == -1)
    {
        perror("Socket is closed");
        return -1;
    }

	status = send(sockd, &request, sizeof(FsRequest), 0);
	count = recv(sockd, &response, sizeof(FsResponse), 0);

    if (errno != 0) {
        perror("Receiving packets error");
        return -1;
    }

    info(response.answer);
	return response.data.fstat.status;
}

int info (FsAnswer answer)
{
    switch (answer)
    {
        case IF_OK:
            printf("File operation ended with success\n");
            break;
        case EF_CORRUPT_PACKAGE:
            perror("File not send, corrupt package, check your file descriptor or server handler");
            return -1;
        case IF_CONTINUE:
            break;
        case EC_SESSION_TIMED_OUT:
            close(sockd);
            sockd = -1;
            perror("Session in server timed out");
            return -1;
        default:
            break;
    }
    return 0;
}
