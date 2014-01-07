#include "server_zombie_collector.h"

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SWEEP_TIME 10
#define ZOMBIFICATION_PERIOD 1800

sem_t sem_zombie_collector_working;
pthread_t zombie_collector_thread;

int zombie_collector_init() {
    
    sem_init(&sem_zombie_collector_working, 0, 1);
    pthread_create(&zombie_collector_thread, NULL, zombie_collector_thread_func, NULL);
    return 0;
}

void *zombie_collector_thread_func(void *parameters) {
    
    while(1) {
        


        sleep(SWEEP_TIME);
    }
    
    return NULL;
}

int zombie_collector_shutdown() {
    
    // todo
    return 42;
}