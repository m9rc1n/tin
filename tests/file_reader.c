#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "../protocol.h"
#include "../tin_library/tin_library.h"

int main(int argc, char* argv[]) {
	    
	if (argc < 3) {
		fprintf(stderr, "Usage: %s address port_number\n", argv[0]);
		exit(1);
	}

	int sid, port;
	port = atoi(argv[2]);


    sid = fs_open_server(argv[1], port);
    printf("Otwieram session_id %d\n", sid);

    int fd = fs_open (sid, "a.x", O_RDONLY);

    char* buf = (char*) calloc(8, sizeof(char));
    fs_read(sid, fd, buf, 8);

    fs_close(sid, fd);

    printf ("Odczytany string z serwera :\n%s\n", buf);

    fs_close_server(sid);
    printf("Zamykam session_id %d\n", sid);

    return 0;
}
