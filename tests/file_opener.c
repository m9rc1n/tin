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
    char *fn1 = "plik.rd";
    char *fn2 = "plik.wr";
    char *fn3 = "plik.??";

    printf("Podaj port serwera: ");
    scanf("%d", &port);

    sid = fs_open_server("0.0.0.0", port);
    printf("Otwieram session_id %d\n", sid);
    fs_open(sid, fn1, "r");
    fs_open(sid, fn2, "w");
    fs_open(sid, fn2, "r");
    sleep(10);
    fs_close_server(sid);
    printf("Zamykam session_id %d\n", sid);

    return 0;
}
