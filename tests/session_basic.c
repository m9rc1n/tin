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

	if (argc < 3) {
		fprintf(stderr, "Usage: %s address port_number\n", argv[0]);
		exit(1);
	}

    int i, how_many = atoi(argv[3]), sid, port = atoi(argv[2]);

    for(i = 0; i < how_many; ++ i) 
    {
        sid = fs_open_server(argv[1], port);
        
        if(sid < 0)
            return -2;

        if(fs_close_server(sid) < 0)
            return -3;

    }

    return 0;
}
