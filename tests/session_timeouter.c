#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
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

    int sid, port = atoi(argv[2]);

    sid = fs_open_server(argv[1], port);
    
    if(sid < 0)
        return -1;
    
    sleep(2 * 60);
    
    if(fs_open(sid, "foo.c", O_RDONLY) >= 0)
        return -1;    
    
    return 0;
}
