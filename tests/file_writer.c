#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
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

    int sid, fd = 0, port = atoi(argv[2]);


    sid = fs_open_server(argv[1], port);
    
    if(sid < 0)
        return -1;

    FILE* send_file = fopen("a.x", "r+");

    if (send_file == NULL) 
        return -2;

    fseek(send_file, 0, SEEK_END);
    size_t file_size = ftell(send_file);
    fseek(send_file, 0, SEEK_SET);
    
    char* buf = (char*) calloc(file_size, sizeof(char));
    fread (buf, sizeof(char), file_size, send_file);

    fd = fs_open(sid, "b.txt", O_WRONLY | O_CREAT);
    fs_write(sid, fd, buf, file_size);
    fs_close(sid, fd);

    sleep(5);
    fs_close_server(sid);

    free(buf);

    return 0;
}
