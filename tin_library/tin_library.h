#ifndef TIN_LIBRARY_H_INCLUDED
#define TIN_LIBRARY_H_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../protocol.h"

extern int sockd;
extern int port;
extern struct sockaddr_in my_addr; 
extern struct sockaddr_in srv_addr;

int fs_open_server (char* server_address);
int fs_close_server (int server_handler);

int fs_open (int server_handler, char* name, int flags);
int fs_write (int server_handler, int fd, void *buf, size_t len);
int fs_read (int server_handler, int fd, long offset, int whence);
int fs_close (int server_handler, int fd);
// int fs_fstat (int server_handler, int fd, struct stat *buf);
int fs_lock (int server_handler, int fd, int mode);

#endif // TIN_LIBRARY_H_INCLUDED
