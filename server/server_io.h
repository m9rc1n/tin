// server
#ifndef SERVER_IO_H_INCLUDED
#define SERVER_IO_H_INCLUDED

#include "server_const.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../protocol.h"

int s_open (int server_handler, char* name, int flags);
int s_write (int server_handler, int fd, void *buf, size_t len);
int s_read (int server_handler, int fd, long offset, int whence);
int s_close (int server_handler, int fd);
// int s_fstat (int server_handler, int fd, struct stat *buf);
int s_lock (int server_handler, int fd, int mode);

#endif // SERVER_IO_H_INCLUDED
