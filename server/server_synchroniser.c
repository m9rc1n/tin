#include "server_synchroniser.h"

#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>



/*
 * 
 */
struct SyncHashMapElement {
  struct SyncHashMapElement *next;
  struct SyncQuery *element;
};

/*
 * 
 */
struct SyncHashMap {
  struct SyncHashMapElement *hashes[SESSION_HASH_LIST_SIZE];
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
  
  hash %= SESSION_HASH_LIST_SIZE;
  return hash;
};

static int sync_hash_map_init() {
    int i;
    
    for(i = 0; i < SESSION_HASH_LIST_SIZE; ++ i)
        synchroniser_map.hashes[i] = NULL;
    
    return 0;
}
    
static int sync_hash_map_create(char *file_name, struct SyncQuery *result) {
    int hash;
    struct SyncHashMapElement *hash_map_element;
    struct SyncHashMapElement *new_element;
    
    hash = sync_hash_map_hash(file_name);
    hash_map_element = synchroniser_map.hashes[hash];
    
    result = (struct SyncQuery *) calloc(1, sizeof(struct SyncQuery));
    result->file_name = (char *) calloc(strlen(file_name), sizeof(char));
    strcpy(result->file_name, file_name);
    
    result->lock_type = FLOCK_NONE;
    result->readers = 0;
    
    new_element = (struct SyncHashMapElement *) calloc(1, sizeof(struct SyncHashMapElement));
    new_element->next = NULL;
    new_element->element = result;
    
    ++ synchroniser_map.size;
    
    if(hash_map_element == NULL) {
        synchroniser_map.hashes[hash] = new_element;
    } else {
        while(hash_map_element->next != NULL)
            hash_map_element = hash_map_element->next;
            
        hash_map_element->next = new_element;
    }
    
    return 0;
}

static int sync_hash_map_find(char *file_name, struct SyncQuery *result) {
    int hash;
    struct SyncHashMapElement *hash_map_element;
    
    hash = sync_hash_map_hash(file_name);
    hash_map_element = synchroniser_map.hashes[hash];
    
    while(hash_map_element != NULL) {
        if(strcmp(hash_map_element->element->file_name, file_name) == 0) 
            break;
        
        hash_map_element = hash_map_element->next;
    }
    
    if(hash_map_element == NULL)
        sync_hash_map_create(file_name, result);
    
    return 0;
}
/* todo: dokonczyc to */


sem_t sem_synchroniser_access;

int synchroniser_query(char *file_name, struct SyncQuery *result) {  
    sem_wait(&sem_synchroniser_access);
    sync_hash_map_find(file_name, result);    
    sem_post(&sem_synchroniser_access);    
    return 0;
}

int synchroniser_init() { 
    sync_hash_map_init();
    sem_init(&sem_synchroniser_access, 0, 1); 
    return 0;
}

int synchroniser_shutdown() {
    sem_destroy(&sem_synchroniser_access);  
    return 0;
}
