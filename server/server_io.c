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
        response.answer = INFO_OK;
        response.data.open.fd = lock_result;
    }

    free(file_name);
    return sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_write (IncomingRequest *inc_request)
{
    FsResponse response;
    FsWriteC data_c = inc_request->request.data.write;
    SessionFileBuffer* session_buffer = (SessionFileBuffer*) malloc(sizeof(SessionFileBuffer));

    int i, status = 0;
    size_t count = 0;
    size_t parts_number = data_c.parts_number;
    int server_handler = data_c.server_handler;
    int fd = data_c.fd;
    int part_id = data_c.part_id;

    size_t file_size = data_c.buffer_len;
    if (session_check_if_exist(server_handler) == -1)
    {
        VDP0("Session timed out\n");
        free(session_buffer);
        response.answer= INFO_SESSION_TIMED_OUT;
        status = sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
        return -1;
    }

    switch (inc_request->request.command)
    {
        case WRITE:
            VDP1("Session %d allowed to receive file\n", server_handler);
            session_buffer->received_parts = (int*) calloc(parts_number, sizeof(int));
            session_buffer->buffer = (char*) calloc(data_c.buffer_len, sizeof(char));
            // session_buffer->parts_number = parts_number;
            for (int i = 0; i<data_c.buffer_len; i++)
            {
                char a = '-';
                strncpy(session_buffer->buffer+i, &a , 1);
            }
            session_set_buffer(server_handler, fd, session_buffer);
            response.answer = INFO_CONTINUE;
            status = sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
            // errors
            break;
        case RECEIVE_PACKAGES:
            session_buffer = session_get_buffer (server_handler, fd);
            if ( session_buffer == NULL )
            {
                response.answer = FILE_SENDING_ERROR;
                break;
            }
            VDP3("Session %d received part %d/%zu\n", server_handler, part_id, parts_number);
            strncpy(session_buffer->buffer + data_c.part_id * BUF_LEN, data_c.buffer, data_c.buffer_len);
            VDP1("Buffer: \n%s\n", session_buffer->buffer);
            VDP1("Zapisuje Buffer: \n%s\n", data_c.buffer);
            int* current_package = session_buffer->received_parts + data_c.part_id;
            *current_package = 1;
            break;
        case RECEIVED_ALL:
            session_buffer = session_get_buffer (server_handler, fd);
            VDP1("Buffer All: \n%s\n", session_buffer->buffer);
            free(session_buffer->received_parts);
            free(session_buffer->buffer);
            session_buffer->received_parts = NULL;
            session_buffer->buffer = NULL;
            // free
            // check
            // respond
            response.answer= FILE_SENDING_SUCCESS;
            status = sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
            break;
        default:
            break;
    }

    if (response.answer == FILE_SENDING_SUCCESS)
    {
        FILE* new_file = session_get (server_handler, fd);
        if (new_file == NULL)
        {
            VDP0 ("Could not save to file\n");
        } else
        {
            printf ("New file address: %d", new_file);
            fwrite(session_buffer->buffer, sizeof(char), 400, new_file);
            fclose (new_file);
        }
    }

    session_buffer = NULL;

    return 0;//sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_close (IncomingRequest *inc_request)
{
    printf("trrza to popprawiic\n");

    // tutaj se zamknijta plik
    session_unlock_file(inc_request->request.data.close.server_handler, inc_request->request.data.close.fd);

    FsResponse response;
    response.answer = INFO_OK;

    return sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
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
