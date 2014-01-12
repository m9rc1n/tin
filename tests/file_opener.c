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
    int sid, port, f1, f2, f3;
    char *fn1 = "plik.rd";
    char *fn2 = "plik.wr";
    char *fn3 = "plik.??";

    printf("Podaj port serwera: ");
    scanf("%d", &port);

    sid = fs_open_server("0.0.0.0", port);
    printf("Otwieram session_id %d\n", sid);
    
    puts("Próba 1");
    printf("\tFd %d\n", f1 = fs_open(sid, fn1, "r"));
    
    puts("Próba 2");
    printf("\tFd %d\n", f2 = fs_open(sid, fn2, "r"));
    
    /* WyWaLa JaKąŚ bEzSeNsOwNą WaRtOśĆ, bO rEsPoNsE != OpEnEd.
     * trzeba poprawic ten test */
    puts("Próba 3");
    printf("\tFd %d\n", f3 = fs_open(sid, fn3, "w"));
    
    sleep(1);
    
    puts("Próba 4");
    printf("\tFd %d\n", fs_open(sid, fn3, "w"));
    
    sleep(1);
    
    fs_close(sid, f3);
    
    puts("Próba 5");
    printf("\tFd %d\n", fs_open(sid, fn3, "r"));
    
    fs_close(sid, f2);
    fs_close(sid, f1);
    
    puts("Próba 6");
    printf("\tFd %d\n", fs_open(sid, fn3, "r"));
    
    fs_close(sid, f3);
    
    fs_close_server(sid);
    printf("Zamykam session_id %d\n", sid);

    return 0;
}
