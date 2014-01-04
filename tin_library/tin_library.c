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
FsCommand command;
FsAnswer answer;
FsOpenServerS open_server_s;
FsOpenServerC open_server_c;
FsCloseServerS close_server_s;
FsCloseServerC close_server_c;

int fs_open_server (char* server_address)
{
	int addrlen, status, count;
	
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

	addrlen = sizeof(struct sockaddr_in);

	command = OPEN_SERVER;
	sendto (sockd, (int*) &command, sizeof(int), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	addrlen = sizeof(struct sockaddr_in);
	recvfrom(sockd, (FsAnswer*) &answer, sizeof(int), 0, (struct sockaddr*)&srv_addr, &addrlen);
	
	if (answer == OK)
	{
		sendto (sockd, (FsOpenServerC*) &open_server_c, sizeof(FsOpenServerC), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	  	recvfrom(sockd, (FsOpenServerS*) &open_server_s, sizeof(FsOpenServerS), 0, (struct sockaddr*)&srv_addr, &addrlen);
	}
    
	return open_server_s.server_handler;
}

int fs_close_server (int server_handler)
{
	int addrlen, status, count;

	command = CLOSE_SERVER;
	status = sendto (sockd, (int*) &command, sizeof(int), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	addrlen = sizeof(struct sockaddr_in);
	recvfrom(sockd, (int*) &answer, sizeof(int), 0, (struct sockaddr*)&srv_addr, &addrlen);
	if (answer == OK)
	{
		close_server_c.server_handler = server_handler;
		sendto (sockd, (FsCloseServerC*) &close_server_c, sizeof(FsCloseServerC), 0, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
	  	recvfrom(sockd, (FsCloseServerS*) &close_server_s, sizeof(FsCloseServerS), 0, (struct sockaddr*)&srv_addr, &addrlen);
	}

	printf ("Closing server: %d\n", close_server_s.answer); 
	
    close (sockd);
    
	return 0;
}

int fs_open (int server_handler, char* name, int flags)
{
}

int fs_write (int server_handler, int fd, void *buf, size_t len)
{
}

int fs_read (int server_handler, int fd, long offset, int whence)
{
}

int fs_close (int server_handler, int fd)
{
}

/*int fs_fstat (int server_handler, int fd, struct stat *buf)
{
}
*/
int fs_lock (int server_handler, int fd, int mode)
{
}
