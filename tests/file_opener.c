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

    int sid, port, f1, f2, f3, f4, f5, f6;
	port = atoi(argv[2]); 
    
    char *fn1 = "plik.rd";
    char *fn2 = "plik.wr";
    char *fn3 = "plik.??";

    sid = fs_open_server(argv[1], port);
    if(sid < 0)
        return -1;

    f1 = fs_open(sid, fn1, O_RDONLY);
    if(f1 < 0)
        return -2;
    
    f2 = fs_open(sid, fn2, O_RDONLY);
    if(f2 < 0)
        return -3;
    
    f3 = fs_open(sid, fn3, O_WRONLY);
    if(f3 < 0)
        return -4;

    sleep(1);

    f4 = fs_open(sid, fn3, O_WRONLY);
    if(f4 >= 0)
        return -5;
    
    sleep(1);
    
    if(fs_close(sid, f3))
        return -6;
    
    f5 = fs_open(sid, fn3, O_RDONLY);
    if(f5 < 0)
        return -7;
    
    if(fs_close(sid, f2) < 0)
        return -8;
    
    if(fs_close(sid, f1) < 0)
        return -9;
  
    f6 = fs_open(sid, fn3, O_RDONLY);
    if(f6 < 0)
        return -10;

    if(fs_close(sid, f1) < 0)
        return -11;

    if(fs_close_server(sid) < 0)
        return -12;

    return 0;
}
