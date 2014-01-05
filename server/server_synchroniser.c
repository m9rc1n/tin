#include "server_synchroniser.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define HASH_LIST_SIZE 31

//////////////////////////////////////////////////////////////////////////////////////////////////

/* Semafory. */

/* Semafor odpowiedzialny za dostep do operacji wloz/wyjmij na kolejce. */
static sem_t sem_request_queue;

/* Semafor ktory ladnie wyglada. */
static sem_t sem_sync_thread;

/*
 * Przygotowuje semafory.
 */ 
static int sync_sem_init() {
  sem_init(&sem_request_queue, 0, 1);
  sem_init(&sem_sync_thread, 0, 0);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////

/* Kolejka zadan uzywana do synchronizacji synchronizatora. */

/*
 * Pojedynczy element kolejki.
 */
struct SyncRequestQueueElement {
  struct SyncRequestQueueElement *next;
  struct SyncRequest *element;
};

/*
 * Kolejka.
 */
struct SyncRequestQueue {
  struct SyncRequestQueueElement *head;
  struct SyncRequestQueueElement *tail;
};  

/* Kolejka zadan synchronizatora. */
static struct SyncRequestQueue synchroniser_requests_queue;

/*
 * Przygotowuje kolejke do dzialania. 
 */ 
static int sync_queue_init() {
  synchroniser_requests_queue.head = NULL;
  synchroniser_requests_queue.tail = NULL;
};

/*
 * Dodaje element na koniec kolejki zadan synchronizatora. Odwiesza watek synchronizatora.
 */
static int sync_queue_add(struct SyncRequest *request) {
  
  struct SyncRequestQueueElement *new_queue_element = (struct SyncRequestQueueElement *) calloc(1, sizeof(struct SyncRequestQueueElement));
  new_queue_element->element = request;
  new_queue_element->next = NULL;
  
  sem_wait(&sem_request_queue);
  
  if(synchroniser_requests_queue.head == NULL) {
    synchroniser_requests_queue.head = new_queue_element;
    synchroniser_requests_queue.tail = new_queue_element;
  } else {
    synchroniser_requests_queue.tail->next = new_queue_element;
    synchroniser_requests_queue.tail = new_queue_element;
  }
  
  sem_post(&sem_sync_thread);
  sem_post(&sem_request_queue);
  
  return 42;
};

/*
 * Wyciaga pierwszy element z kolejki zadan synchronizatora. Watek sam sie musi potem zawiesic.
 */
static int sync_queue_get(struct SyncRequest *request) {
  
  sem_wait(&sem_sync_thread);
  sem_wait(&sem_request_queue);
 
  struct SyncRequestQueueElement *first_queue_element = synchroniser_requests_queue.head;
  synchroniser_requests_queue.head = first_queue_element->next;
  
  if(first_queue_element->next == NULL)
    synchroniser_requests_queue.tail = NULL;
    
  sem_post(&sem_request_queue);
  
  request = first_queue_element->element;
  free(first_queue_element);
  
  return 42;
};

//////////////////////////////////////////////////////////////////////////////////////////////////

/* Kontener na wszelkiego rodzaju blokady na plikach. */ 

/*
 * 
 */
struct SyncHashMapElement {
  struct HashMapElement *prev;
  struct HashMapElement *next;
  
  struct SyncRequest *element;
};

/*
 * 
 */
typedef struct SyncHashMap {
  struct HashMapElement *hashes[HASH_LIST_SIZE];
  unsigned size;
};

/* */
static struct SyncHashMap synchroniser_map;

/*
 * 
 */
static int sync_hash_map_hash(char *file_name) {
  int i;
  int hash = 0;
  
  for(i = 0; i < strlen(file_name); ++ i) {
    hash += (int) file_name[i];
  }
  
  hash %= HASH_LIST_SIZE;
  return hash;
};

/* todo: dokonczyc to */


//////////////////////////////////////////////////////////////////////////////////////////////////

/* Watek synchronizatora i jemu potrzebne rzeczy. */

static pthread_t synchroniser_thread;

static void *synchroniser_thread_func(void *parameters) {
  struct SyncRequest *rq;

  while(1) {
    sync_queue_get(rq);

    // tu sie dzieje magia
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////

/* Intefrejs synchronizatora dla reszty aplikacji. */

/*
 * Dodaje zadanie do kolejki synchronizatora.
 */
int synchroniser_add(struct SyncRequest *request) {
  
  return sync_queue_add(request);
}

int synchroniser_init() {
 
  sync_queue_init();
  sync_sem_init();
  
  pthread_create(&synchroniser_thread, NULL, synchroniser_thread_func, NULL);
 
  return 42;
}

int synchroniser_shutdown() {
  pthread_join(synchroniser_thread, NULL);
}
