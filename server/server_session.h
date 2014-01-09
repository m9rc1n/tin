#ifndef SEVER_SESSION_H
#define SEVER_SESSION_H

#include "server_const.h"

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

/**
 * Zamyka zarządcę sesji.
 */
int session_shutdown();

#endif
