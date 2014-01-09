#ifndef _SERVER_THREAD_H
#define _SERVER_THREAD_H

#include "server_const.h"
#include "server_network.h"
#include "server_io.h"

#include <pthread.h>
#include <stdio.h>

void *server_thread_function(void *parameters);

#endif
