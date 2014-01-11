#include "../protocol.h"
#include "server_io.h"
#include "server_network.h"
#include "server_synchroniser.h"

int s_open(IncomingRequest *inc_request) {
    char *file_name = (char *) calloc(inc_request->request.data.open.name_len, sizeof(char));
    strncpy(file_name, inc_request->request.data.open.name, inc_request->request.data.open.name_len);

   /** @todo faktyczna obsługa tych plików */


    /** @fixme tych flag nie widzę zdefiniowanych nigdzie. */
    /** @fixme to chyba nie miało tak wyglądać... :P */
    if(strcmp(inc_request->request.data.open.mode , "r"))
        printf("imma readin: %d\n", session_lock_file(inc_request->request.data.open.server_handler, file_name, FLOCK_READ));
    else
        printf("imma writin: %d\n", session_lock_file(inc_request->request.data.open.server_handler, file_name, FLOCK_WRITE));

    FsResponse response;
    response.answer = OK;

    free(file_name);
    return sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_write (IncomingRequest *inc_request)
{
    printf ("\nBuffer: %zu : %s\n", inc_request->request.data.write.buffer_len, inc_request->request.data.write.buffer);

    FsResponse response;
    response.answer = OK;

    return sendto(sockd, &response, sizeof(FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_close (IncomingRequest *inc_request)
{
    return -1;
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
