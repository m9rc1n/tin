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

    int fd = fs_open (sid, "a.x", "r");

    char* buf = (char*) calloc(8, sizeof(char));
    fs_read(sid, fd, buf, 8);

    fs_close(sid, fd);

    printf ("Odczytany string z serwera :\n%s\n", buf);

    fs_close_server(sid);
    printf("Zamykam session_id %d\n", sid);

    return 0;
}
