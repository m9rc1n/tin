// client
#ifndef CLIENT_IO_H_INCLUDED
#define CLIENT_IO_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../protocol.h"

int c_open (int server_handler, char* name, int flags);
int c_write (int server_handler, int fd, void *buf, size_t len);
int c_read (int server_handler, int fd, long offset, int whence);
int c_close (int server_handler, int fd);
// int c_fstat (int server_handler, int fd, struct stat *buf);
int c_lock (int server_handler, int fd, int mode);


#endif // CLIENT_IO_H_INCLUDED
