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
    
    FsResponse response;
    
    if(lock_result < 0) {
        /* Nie udało się dostać blokady na plik - trzeba powiadomić o tym klienta. */
        /* @todo ja się nie znam na protokole tak dobrze, jak Wy - może trzeba ucywilizować. ~ AK */
        response.answer = FILE_BLOCKED;
        
    } else {
        response.answer = OK;
    }
    
    free(file_name);
    return sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_write (IncomingRequest *inc_request)
{
    FsWriteC data_c = inc_request->request.data.write;
    int i, status = 0;
    size_t count = 0;
    size_t parts_number = data_c.parts_number;
    size_t last_part = data_c.buffer_len%BUF_LEN;
    int server_handler = data_c.server_handler;
    int fd = data_c.fd;

    int* received_packages = (int*) calloc(parts_number, sizeof(int));
    char* buffer = (char*) calloc(data_c.buffer_len, sizeof(char));

    for (i=0; i<parts_number; i++)
    {
        count = recvfrom(sockd, (char*) (&inc_request->request), MAX_BUF, 0, (struct sockaddr*) &(inc_request->client_addr), &(inc_request->client_addr_len));
        if (inc_request->request.command == RECEIVE_PACKAGES)
        {
            data_c = inc_request->request.data.write;
            if (data_c.fd == fd && data_c.server_handler == server_handler && data_c.parts_number == parts_number)
            {
                memcpy(buffer+data_c.part_id*BUF_LEN*sizeof(char), data_c.buffer, data_c.buffer_len*sizeof(char));
                int* current_package = received_packages+data_c.part_id*sizeof(int);
                *current_package = 1;
            }
        }
    }

    FsResponse response;
    response.answer = OK;

    for (i=9; i<parts_number; ++i)
    {
        int* current_package = received_packages+i*sizeof(int);
        if (current_package == 0 ) response.answer = SENDING_ERROR;
    }

    printf ("\nBuffer : %s\n", buffer);

    free(received_packages);
    free(buffer);
    // wszystkie paczki przeslane

    return sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_close (IncomingRequest *inc_request)
{
    return -1;
}

// Otworz plik o identyfikatorze fd
// Przeslij na wskazany adres w czesciach

int s_read (IncomingRequest *inc_request)
{
    FsReadC data_c = inc_request->request.data.read;
    // printf ("\nBuffer_len: %zu id: %d: %s\n", data_c.buffer_len, data_c.part_id, data_c.buffer);
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
