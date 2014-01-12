#include "server_session.h"
#include "server_synchroniser.h"
#include <assert.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

struct SessionLock {
    char *file_name;
    FILE *fh;
    int fd;
    FileLockType lock_type;
};

struct Session {
    unsigned id;
    time_t time_active;
    struct sockaddr_in client_addr;
    unsigned int client_addr_len;

    struct SessionLock *locks[MAX_FD_PER_SESSION];
};

/** Lista aktywnych (jeszcze...) sesji. */
static struct Session *sessions_list[SESSION_MAX_NUMBER];

static int session_add_lock(int session_id, char *file_name, FileLockType file_lock_type) {
    int i;
    
    for(i = 0; i < MAX_FD_PER_SESSION; ++ i) 
        if(sessions_list[session_id]->locks[i] == NULL)
            break;
        
    if(i == MAX_FD_PER_SESSION)
        return -1;
    
    VDP2("Setting lock on file %s, SID: %d\n", file_name, session_id);   
    
    struct SessionLock *session_lock = (struct SessionLock *) calloc(1, sizeof(struct SessionLock));
    session_lock->file_name = (char *) calloc(strlen(file_name), sizeof(char));
    strncpy(session_lock->file_name, file_name, strlen(file_name));
    session_lock->lock_type = file_lock_type;
    session_lock->fd = i;
    session_lock->fh = NULL;
    
    sessions_list[session_id]->locks[i] = session_lock;
    
    return i;
}

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

static int session_remove_lock(unsigned session_id, unsigned fd) {
    if(sessions_list[session_id] == NULL)
        return -1;
    
    if(sessions_list[session_id]->locks[fd] == NULL)
        return -2;
    
    VDP2("Unlocking file %s SID: %d\n", sessions_list[session_id]->locks[fd]->file_name, session_id);
            
    struct SyncQuery *sq = NULL;    
    char *file_name = sessions_list[session_id]->locks[fd]->file_name;

    if(synchroniser_query(file_name, &sq) < 0)
        return -1;
    
    switch(sessions_list[session_id]->locks[fd]->lock_type) {
        
        case FLOCK_READ:
    
            -- sq->readers;
            
            if(sq->readers == 0) {
                if(sq->lock_type == FLOCK_WRITE_PENDING) {
                    //sq->lock_type = FLOCK_WRITE;
                    /** @todo można już pisać - coś trzeba z tym fantem zrobić. */
                } else {
                    sq->lock_type = FLOCK_NONE;
                }
            }
            
            break;
            
        case FLOCK_WRITE_PENDING:
            
            if(sq->readers == 0) {
                sq->lock_type = FLOCK_NONE;
            } else {
                sq->lock_type = FLOCK_READ;
            }
            
            break;
            
        case FLOCK_WRITE:
            
            sq->lock_type = FLOCK_NONE;
            break;
            
        default:
            assert(0);
    }
    
    
    free(sessions_list[session_id]->locks[fd]->file_name);
    free(sessions_list[session_id]->locks[fd]);
    sessions_list[session_id]->locks[fd] = NULL;
    return 0;
}

/**
 * Zdejmuje blokady nałożone przez daną sesję.
 */
static int session_remove_locks(unsigned session_id) {
    int i;
    
    if(sessions_list[session_id] == NULL)
        return -1;
    
    for(i = 0; i < MAX_FD_PER_SESSION; ++ i) {
        if(sessions_list[session_id]->locks[i] == NULL) 
            session_remove_lock(session_id, i);             
    }
    
    VDP1("Unlocked files from %d sesssion.\n", session_id);    
    return 0;
}


/** Semafor blokujący modyfikację listy sesji. */
static sem_t sem_sessions_list_access;

////////////////////////////////////////////////////////////////////////////////////////////////////

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

        sem_post(&sem_sessions_list_access);
        return -1;
    }

    if(sessions_list[session_id] == NULL) {

        sem_post(&sem_sessions_list_access);
        return -2;
    }

    session_remove_locks(session_id);
    free(sessions_list[session_id]);
    sessions_list[session_id] = NULL;

    sem_post(&sem_sessions_list_access);

    VDP1("Session %d closed.\n", session_id);
    
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

    VDP1("Creating session SID: %d\n", session_id);
    
    sessions_list[session_id] = (struct Session *) calloc(1, sizeof(struct Session));
    sessions_list[session_id]->id = session_id;
    sessions_list[session_id]->time_active = time(NULL);
    sessions_list[session_id]->client_addr_len = sizeof(struct sockaddr_in);

    for(int i = 0; i < MAX_FD_PER_SESSION; ++ i)
        sessions_list[session_id]->locks[i] = NULL;
    
    memcpy(&sessions_list[session_id]->client_addr, &client_addr, sizeof(struct sockaddr_in));

    sem_post(&sem_sessions_list_access);
    return session_id;
}

int session_destroy_zombies() {
    unsigned i;

    sem_wait(&sem_sessions_list_access);

    for(i = 0; i < SESSION_MAX_NUMBER; ++ i) {

        if(sessions_list[i] != NULL) {

            if((time(NULL) - sessions_list[i]->time_active) > SESSION_ZOMBIE_TIME) {

                /* Send REQUEST_TIMEOUT maybe? Definatelly not spam std out. */
                printf("WAAAGH, SEZION NUMAH %d IZ DESTROYYY'D\n", i);
                session_remove_locks(i);
                free(sessions_list[i]);
                sessions_list[i] = NULL;
            }
        }
    }

    sem_post(&sem_sessions_list_access);

    return 0;
}

FILE *session_get(int session_id, int fd) {
    
    /* todo moar ifz */
    
    if(sessions_list[session_id] == NULL)
        return NULL;
    
    if(sessions_list[session_id]->locks[fd] == NULL)
        return NULL;
    
    return sessions_list[session_id]->locks[fd]->fh;
}

int session_set(int session_id, int fd, FILE *fh) {
    if(sessions_list[session_id] == NULL)
        return -1;
    
    if(sessions_list[session_id]->locks[fd] == NULL)
        return -2;
    
    sessions_list[session_id]->locks[fd]->fh = fh;
    return 0;
}

int session_lock_file(int session_id, char *file_name, FileLockType file_lock_type) {
    
    struct SyncQuery *sq = NULL;    
    
    if(synchroniser_query(file_name, &sq) < 0)
        return -1;
    
    assert(sq != NULL);
    
    VDP2("File %s has %d readers.\n", sq->file_name, sq->readers);
    
    switch(file_lock_type) {
        
        case FLOCK_READ:
            VDP0("Attempting READ lock.\n");
            
            if(sq->lock_type == FLOCK_READ || sq->lock_type == FLOCK_NONE) {
                
                sq->lock_type = FLOCK_READ;
                ++ sq->readers;
                return session_add_lock(session_id, file_name, file_lock_type);
            } else {
                
                return -2;
            }
            
            break;
            
        case FLOCK_WRITE:
            VDP0("Attempting WRITE lock.\n");
            
            if(sq->lock_type == FLOCK_NONE) {
                
                sq->lock_type = FLOCK_WRITE;
                return session_add_lock(session_id, file_name, file_lock_type);
            } else if(sq->lock_type == FLOCK_READ) {
                
                sq->lock_type = FLOCK_WRITE_PENDING;
                return session_add_lock(session_id, file_name, file_lock_type);
            } else {
                
                return -3;
            }
            
            break;
            
        default:
            assert(0);         
    }
    
    return -4;
}

int session_unlock_file(int session_id, int fd) {
    return session_remove_lock(session_id, fd);
}

int session_shutdown() {
    int i;

    sem_destroy(&sem_sessions_list_access);

    for(i = 0; i < SESSION_MAX_NUMBER; ++ i)
        if(sessions_list[i] != NULL)
            session_close(i);

    return 0;
}
