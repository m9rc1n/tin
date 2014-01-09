#include "server_session.h"
#include "server_zombie_collector.h"

#include <pthread.h>
#include <unistd.h>

pthread_t zombie_collector_thread;

int zombie_collector_init() {

    pthread_create(&zombie_collector_thread, NULL, zombie_collector_thread_func, NULL);
    return 0;
}

void *zombie_collector_thread_func(void *parameters) {
    
    while(1) {
        session_destroy_zombies();
        sleep(SESSION_SWEEP_TIME);
    }
    
    return NULL;
}

int zombie_collector_shutdown() {
    
    pthread_join(zombie_collector_thread, NULL);
    return 0;
}