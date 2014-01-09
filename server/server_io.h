// server
#ifndef SERVER_IO_H_INCLUDED
#define SERVER_IO_H_INCLUDED

#include "server_const.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../protocol.h"
#include "server_protocol.h"

int s_open (IncomingRequest *inc_request);
int s_write (IncomingRequest *inc_request);
int s_read (IncomingRequest *inc_request);
int s_close (IncomingRequest *inc_request);
int s_fstat (IncomingRequest *inc_request);
int s_lock (IncomingRequest *inc_request);
int s_lseek (IncomingRequest *inc_request);

#endif // SERVER_IO_H_INCLUDED
