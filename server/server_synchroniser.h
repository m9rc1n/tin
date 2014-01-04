#ifndef SERVER_SYNCHRONISER_H
#define SERVER_SYNCHRONISER_H

struct SyncRequest {
  char *file_name; //todo reszta
};

extern int synchroniser_add(struct SyncRequest *request);

extern int synchroniser_init();

#endif
