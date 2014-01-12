#include "../protocol.h"
#include "server_io.h"
#include "server_network.h"
#include "server_synchroniser.h"
#include <stdio.h>

int s_open(IncomingRequest *inc_request) {
    int lock_result;
    
    char *file_name = (char *) calloc(inc_request->request.data.open.name_len, sizeof(char));
    strncpy(file_name, inc_request->request.data.open.name, inc_request->request.data.open.name_len);

    VDP2("Incoming OPEN request: %s (mode: %s)\n", file_name, inc_request->request.data.open.mode);
    
   /** @todo faktyczna obsługa tych plików */


    /** @fixme tych flag nie widzę zdefiniowanych nigdzie. */
    /** @fixme to chyba nie miało tak wyglądać... :P */

    /* Ludzie, kurde, to jest C, ono nie działa intuicyjnie. 
     *  http://pl.wikibooks.org/wiki/C/strcmp 
     */
    if(strcmp(inc_request->request.data.open.mode, "r") == 0)
        lock_result = session_lock_file(inc_request->request.data.open.server_handler, file_name, FLOCK_READ);
    else
        lock_result = session_lock_file(inc_request->request.data.open.server_handler, file_name, FLOCK_WRITE);
    
    // fopen(qwerty)
    // session_set(sid, fd, FILE) 
    
    FsResponse response;
    
    if(lock_result < 0) {
        /* Nie udało się dostać blokady na plik - trzeba powiadomić o tym klienta. */
        /* @todo ja się nie znam na protokole tak dobrze, jak Wy - może trzeba ucywilizować. ~ AK */
        VDP0("Lock failed, request turned down.\n");
        response.answer = FILE_BLOCKED;
        
    } else {
        VDP0("Lock accepted, request accepted.\n");
        response.answer = OK;
        response.data.open.fd = lock_result;
    }
    
    free(file_name);
    return sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_write (IncomingRequest *inc_request)
{
    printf ("\nBuffer_len: %zu id: %d: %s\n", inc_request->request.data.write.buffer_len, inc_request->request.data.write.part_id, inc_request->request.data.write.buffer);

    FsResponse response;
    response.answer = OK;

    return sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_close (IncomingRequest *inc_request)
{
    printf("trrza to popprawiic\n");
    
    // tutaj se zamknijta plik
    session_unlock_file(inc_request->request.data.close.server_handler, inc_request->request.data.close.fd);
    
    FsResponse response;
    response.answer = OK;

    return sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_read (IncomingRequest *inc_request)
{
    return -1;
}

int s_fstat (IncomingRequest *inc_request)
{
    return -1;
}

int s_lock (IncomingRequest *inc_request)
{
    return -1;
}

int s_lseek (IncomingRequest *inc_request)
{
    return -1;
}
