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
 * Zamyka istniejącą sesję.
 */
int session_close(int session_id);

/**
 * Dodaje nową sesję, zwraca jej identyfikator.
 */
int session_create();

/**
 * Wykonuje zombie collection.
 */
int session_destroy_zombies();

FILE *session_get(int session_id, int fd);

/**
 * Próbuje założyć podaną blokadę na plik. Ujemny kod wyjścia oznacza niepowodzenie.
 * 
 * @todo kody błędów
 */
int session_lock_file(int session_id, char *file_name, FileLockType file_lock_type) ;

int session_set(int session_id, int fd, FILE *fh);

/**
 * Zamyka zarządcę sesji.
 */
int session_shutdown();

int session_unlock_file(int session_id, int fd);

#endif
