#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../protocol.h"
#include "../tin_library/tin_library.h"

int main(int argc, char* argv[]) {
    int sid, port;

    printf("Podaj port serwera: ");
    scanf("%d", &port);

    sid = fs_open_server("0.0.0.0", port);
    printf("Otwieram session_id %d\n", sid);

    FILE* sendFile = fopen("a.x", "r+");

    printf ("Wsk: %d\n", sendFile);

    // char* buf = (char*) calloc(1024, sizeof(char));
    char buf[1024];
    fread (&buf, sizeof(char), 1024, sendFile);

    fs_write(sid, 10, buf, sizeof(buf));
    fs_close_server(sid);
    printf("Zamykam session_id %d\n", sid);

    return 0;
}
