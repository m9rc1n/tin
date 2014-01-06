#ifndef SERVER_SYNCHRONISER_H
#define SERVER_SYNCHRONISER_H

struct SyncQuery {
  char *file_name; //todo reszta
};

/**
 * 
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
