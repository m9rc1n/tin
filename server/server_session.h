#ifndef SEVER_SESSION_H
#define SEVER_SESSION_H

struct Session {
    unsigned id;
    unsigned time_active;
    // todo założone blokady

    struct Session *prev;
    struct Session *next;
};

extern struct Session *session_list;

int session_init();

int session_shutdown();


#endif
