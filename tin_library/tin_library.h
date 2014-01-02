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

#define MAX_BUF 100

int fs_open_server (char* server_address);
int fs_close_server (int srvhndl);

int fs_open (int srvhndl, char* name, int flags);
int fs_write (int srvhndl, int fd, void *buf, size_t len);
int fs_read (int srvhndl, int fd, long offset, int whence);
int fs_close (int srvhndl, int fd);
/*int fs_fstat (int srvhndl, int fd, struct stat *buf);
*/int fs_lock (int srvhndl, int fd, int mode);

#endif // TIN_LIBRARY_H_INCLUDED
