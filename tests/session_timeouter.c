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

    printf ("Podaj nr portu: ");
    scanf ("%d", &port);

    sid = fs_open_server("0.0.0.0", port);
    printf("Otwieram, session_id %d\n", sid);

    sleep(3600);
    return 0;
}
