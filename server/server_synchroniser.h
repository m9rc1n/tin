#ifndef SERVER_SYNCHRONISER_H
#define SERVER_SYNCHRONISER_H

typedef enum FileLockType {
    FLOCK_NONE, 
    FLOCK_READ, 
    FLOCK_WRITE
} FileLockType;

/**
 * 
 */
struct SyncQuery {
  char *file_name;  
  FileLockType lock_type;
  unsigned readers;
};

/**
 * Pyta bazę synchronizatora o plik o podanej nazwie. Odpowiedź znajduje się w *result.
 */
extern int synchroniser_query(char *file_name, struct SyncQuery *result);

/**
 * 
 */
extern int synchroniser_init();

/**
 * 
 */
extern int synchroniser_shutdown();

#endif
