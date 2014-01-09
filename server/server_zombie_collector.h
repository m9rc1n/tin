#ifndef SERVER_ZOMBIE_COLLECTOR_H
#define SERVER_ZOMBIE_COLLECTOR_H

#include "server_const.h"
#include <semaphore.h>

extern sem_t sem_zombie_collector_working;
extern pthread_t zombie_collector_thread;

int zombie_collector_init();
void *zombie_collector_thread_func(void *parameters);
int zombie_collector_shutdown();

#endif
