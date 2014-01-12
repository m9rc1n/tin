#ifndef SEVER_SESSION_H
#define SEVER_SESSION_H

#include "server_const.h"
#include <stdio.h>

/**
 * Możliwe blokady do nałożenia na plik.
 *  FLOCK_NONE - blokad nie ma wcale.
 *  FLOCK_READ - dozwolone jest tylko czytanie.
 *  FLOCK_WRITE_PENDING - obecnie plik jest czytany, ale w kolejce jest pisarz.
 *  FLICK_WRITE - pisarz zajął plik.
 */
typedef enum FileLockType {
    FLOCK_NONE, 
    FLOCK_READ, 
    FLOCK_WRITE_PENDING,
    FLOCK_WRITE
} FileLockType;

/**
 * Inicjuje zarządcę sesji. 
 */
int session_init();

/**
 * Bumpuje sesję (odznacza, że coś się działo). Jak nie będziecie tego używać, to zombie collection
 *  wywali sesję z pamięci. ;)
 * 
 * @todo być może zaimplementować w pozostałych session_* na wszelki wypadek, ale sami lepiej też 
 *  tego używajcie
 * 
 * -1 - sesja nie istnieje
 * -3 - niepoprawne sid
 */
int session_bump(int session_id);

/**
 * Zamyka istniejącą sesję.
 * 
 * -1 - sesja nie istnieje
 * -3 - niepoprawne sid
 */
int session_close(int session_id);

/**
 * Dodaje nową sesję, zwraca jej identyfikator.
 * 
 * wszystko >= 0 - nadane session_id
 * 
 * -1 - wyczerpany limit sesji
 */
int session_create();

/**
 * Wykonuje zombie collection.
 */
int session_destroy_zombies();

/**
 * Oddaje wskaźnik na FILE dla konkretnego pliku.
 *  Jeżeli nastąpił błąd (zły sid, zły fd), zwraca NULL.
 */
FILE *session_get(int session_id, int fd);

/**
 * Próbuje założyć podaną blokadę na plik i nadać mu fd. Ujemny kod wyjścia oznacza niepowodzenie.
 * 
 * wszystko >= 0 - nadany fd
 *  
 * -1 - sesja nie istnieje
 * -2 - fd nie istnieje
 * -3 - niepoprawne sid/fd
 * -4 - błąd wewnętrzny
 * -5 - wyczerpany limit fd dla sesji
 * -6 - nie da się zablokować - ktoś już pisze do pliku
 * -7 - nie da się zablokować - ktoś czyta z pliku
 */
int session_lock_file(int session_id, char *file_name, FileLockType file_lock_type) ;

/**
 * Ustawia wskaźnik FILE dla konkretnego pliku.
 *  Dobrze wywołać po fopen().
 * 
 *  0 - wszystko ok
 * -1 - sesja nie istnieje
 * -2 - fd nie istnieje
 * -3 - niepoprawne sid/fd
 */
int session_set(int session_id, int fd, FILE *fh);

/**
 * Zamyka zarządcę sesji.
 */
int session_shutdown();

/**
 * Odblokowuje plik (zdejmuje blokadę założoną przez odpowiednią sesję.
 * 
 *  0 - wszystko ok
 * -1 - sesja nie istnieje
 * -2 - fd nie istnieje
 * -3 - niepoprawne sid/fd
 * -4 - błąd wewnętrzny
 */
int session_unlock_file(int session_id, int fd);

#endif
