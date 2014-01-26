#ifndef SERVER_CONST_H
#define SERVER_CONST_H

#include "server_log.h"

#define VDP0(x) sprintf(logbuffer, x); logprint();
#define VDP1(x,y) sprintf(logbuffer, x, y); logprint();
#define VDP2(x,y,z) sprintf(logbuffer, x, y, z); logprint();
#define VDP3(x,y,z,q) sprintf(logbuffer, x, y, z, q); logprint();
#define VDP4(x,y,z,q,w) sprintf(logbuffer, x, y, z, q, w); logprint();


/**
 * Tutaj wrzucamjmy wszystkie makrodefinicje, żeby było potem prościej zmieniać wszelkiego rodzaju
 *  parametry serwera. Ponadto raczej lepiej definiować parametry poprzez idndef-define-endif, bo
 *  wtedy można dać dla testów jakiś mniej sensowny parametr i wygodniej testować (np. timeouty).
 */

/**
 * Stop receiving on socket after that time in seconds
 */
#ifndef WAIT_TO_STOP_RCV
#define WAIT_TO_STOP_RCV 120
#endif

/**
 * Wielkość tablicy w hashmapie. Używane w obsłudze sesji.
 * Z definicji hashpamy lepiej podać liczbę pierwszą.
 */
#ifndef SESSION_HASH_LIST_SIZE
#define SESSION_HASH_LIST_SIZE 31
#endif

/**
 * Maksymalna ilość jednocześnie aktywnych sesji. Używane w obsłudze sesji.
 */
#ifndef SESSION_MAX_NUMBER
#define SESSION_MAX_NUMBER 16
#endif

/**
 * Okres wykonywania zombie collection (ubijania starych i nieaktywnych sesji) w sekundach.
 */
#ifndef SESSION_SWEEP_TIME
#define SESSION_SWEEP_TIME 10
#endif

/**
 * Okres od ostatniego requesta po upłynięciu którego sesja jest uznana jako zombie w sekundach.
 */
#ifndef SESSION_ZOMBIE_TIME
#define SESSION_ZOMBIE_TIME 300
#endif

/**
 * Maksymalna ilość plików otwartych w jednej sesji.
 */
#ifndef MAX_FD_PER_SESSION
#define MAX_FD_PER_SESSION 4
#endif

#endif
