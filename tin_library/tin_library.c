#include "tin_library.h"

/**
 * UDP jest protokolem bezpolaczeniowym, 
 * wiec ta funkcja sluzy nam 
 * do zarejestrowania klienta w serwerze
 *
 * wysylamy login a serwer jesli odpowie dobrze to zwracamy uchwyt
 */ 
int fs_open_server (char* server_address)
{
	int sockd;
    struct sockaddr_in my_addr, srv_addr;
    char buf[MAX_BUF];
    int count;
    int addrlen;

	FsOpenServerS server_struct;
	FsOpenServerC client_struct = {OPEN_SERVER, server_address};
	
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

    bind(sockd, (struct sockaddr*)&my_addr, sizeof(my_addr));

    /* server address */
    srv_addr.sin_family = AF_INET;
    inet_aton("localhost", &srv_addr.sin_addr);
    srv_addr.sin_port = htons(atoi("61110"));
	
	sendto(sockd, &client_struct, sizeof(client_struct), 0, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    addrlen = sizeof(srv_addr);

//    	count = recvfrom(sockd, &server_struct, sizeof(server_struct), 0, (struct sockaddr*)&srv_addr, &addrlen);

    close(sockd);

	return server_struct.server_handler;
}

int fs_close_server (int srvhndl)
{
}

int fs_open (int srvhndl, char* name, int flags)
{
}

int fs_write (int srvhndl, int fd, void *buf, size_t len)
{
}

int fs_read (int srvhndl, int fd, long offset, int whence)
{
}

int fs_close (int srvhndl, int fd)
{
}

/*int fs_fstat (int srvhndl, int fd, struct stat *buf)
{
}
*/
int fs_lock (int srvhndl, int fd, int mode)
{
}
