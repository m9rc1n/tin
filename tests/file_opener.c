#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <curses.h>

#include "../protocol.h"
#include "../tin_library/tin_library.h"

int main(int argc, char* argv[]) {
	//int ANS_X = 22, ANS_Y = 1;
	
	if (argc < 3) {
		fprintf(stderr, "Usage: %s address port_number\n", argv[0]);
		exit(1);
	}

    int sid, port, f1, f2, f3;
	port = atoi(argv[2]); 
    char *fn1 = "plik.rd";
    char *fn2 = "plik.wr";
    char *fn3 = "plik.??";

    sid = fs_open_server(argv[1], port);
    printf("Otwieram session_id %d\n", sid);

    puts("Próba 1");
    printf("\tFd %d\n", f1 = fs_open(sid, fn1, O_RDONLY));


    puts("Próba 2");
    printf("\tFd %d\n", f2 = fs_open(sid, fn2, O_RDONLY));

    puts("Próba 3");
    printf("\tFd %d\n", f3 = fs_open(sid, fn3, O_WRONLY));

    sleep(1);

    puts("Próba 4");
    printf("\tFd %d\n", fs_open(sid, fn3, O_WRONLY));

    sleep(1);

    fs_close(sid, f3);

    puts("Próba 5");
    printf("\tFd %d\n", fs_open(sid, fn3, O_RDONLY));

    fs_close(sid, f2);
    fs_close(sid, f1);

    puts("Próba 6");
    printf("\tFd %d\n", fs_open(sid, fn3, O_RDONLY));

    fs_close(sid, f1);

    fs_close_server(sid);
    printf("Zamykam session_id %d\n", sid);

    return 0;
}
