#include "server_session.h"
#include <netinet/in.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum SessionLockType {
    SLT_READ,
    SLT_WRITE,
} SessionLockType;

struct SessionLock {
    char *file_name;
    SessionLockType lock_type;
};

struct Session {
    unsigned id;
    time_t time_active;
    struct sockaddr_in client_addr;
    unsigned int client_addr_len;
   
    struct SessionLock *locks;
};

/** Lista aktywnych (jeszcze...) sesji. */
static struct Session *sessions_list[SESSION_MAX_NUMBER];

/**
 * Zwraca pierwsze wolne id sesji lub -1, jeżeli takowe nie istnieje.
 */
static int session_find_free_id() {
    int i;

    for(i = 0; i < SESSION_MAX_NUMBER; ++ i)
        if(sessions_list[i] == NULL) 
            return i;
        
    return -1;
}

/**
 * Zdejmuje blokady nałożone przez daną sesję.
 */
static int session_remove_locks(unsigned session_id) {
    
    return 42;
}
    

/** Semafor blokujący modyfikację listy sesji. */
static sem_t sem_sessions_list_access;

int session_init() {
    int i;
    
    for(i = 0; i < SESSION_MAX_NUMBER; ++ i) 
        sessions_list[i] = NULL;
    
    sem_init(&sem_sessions_list_access, 0, 1);
    
    return 0;
}

int session_close(int session_id) {
    
    sem_wait(&sem_sessions_list_access);

    if(session_id >= SESSION_MAX_NUMBER) {
        //printf("wartosc %d jest z (_!_) wzieta\n", session_id);
        sem_post(&sem_sessions_list_access);  
        return -1;
    }
    
    if(sessions_list[session_id] == NULL) {
        //printf("wartosc %d jest z (_!_) nieaktualna\n", session_id);
        sem_post(&sem_sessions_list_access);  
        return -2;
    }
    
    session_remove_locks(session_id);
    free(sessions_list[session_id]);
    sessions_list[session_id] = NULL;
    
    sem_post(&sem_sessions_list_access);  
    
    return 0;
}

int session_create(struct sockaddr_in client_addr, unsigned client_addr_len) {
    unsigned session_id;

    sem_wait(&sem_sessions_list_access);
    session_id = session_find_free_id();

    if(session_id == -1)
    {
        sem_post(&sem_sessions_list_access);
        return -1;
    }

    sessions_list[session_id] = (struct Session *) calloc(1, sizeof(struct Session));
    sessions_list[session_id]->id = session_id;
    sessions_list[session_id]->time_active = time(NULL);
    sessions_list[session_id]->locks = NULL;
    sessions_list[session_id]->client_addr_len = sizeof(struct sockaddr_in);

    memcpy(&sessions_list[session_id]->client_addr, &client_addr, sizeof(struct sockaddr_in));

    sem_post(&sem_sessions_list_access);    
    return session_id;
}

int session_destroy_zombies() {
    
    return 42;
}


int session_shutdown() {
    int i;
    
    sem_destroy(&sem_sessions_list_access);
    
    for(i = 0; i < SESSION_MAX_NUMBER; ++ i)
        if(sessions_list[i] != NULL)
            free(sessions_list[i]);
        
    return 0;
}