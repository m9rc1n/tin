#ifndef SERVER_SYNCHRONISER_H
#define SERVER_SYNCHRONISER_H

#include "server_const.h"
#include "server_session.h"


/**
 * Struktura zwracana z synchronizatora.
 */
struct SyncQuery {
    /** Nazwa pliku. */
    char *file_name;  
    
    /** Typ blokady nałożonej na plik. */
    FileLockType lock_type;
    
    /** Ilość czytelników korzystających z pliku. */
    unsigned readers;
};

/**
 * Pyta bazę synchronizatora o plik o podanej nazwie. Odpowiedź znajduje się w *result.
 */
extern int synchroniser_query(char *file_name, struct SyncQuery **result);

/**
 * Inicjuje synchronizator.
 */
extern int synchroniser_init();

/**
 * Zamyka uchwyty i zwalnia pamięć.
 */
extern int synchroniser_shutdown();

#endif
