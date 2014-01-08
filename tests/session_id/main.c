#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../protocol.h"
#include "../../tin_library/tin_library.h"

int main(int argc, char* argv[]) {
    int i, how_many, sid;
    
    printf("Ile powtorzen?\n");
    scanf("%d", &how_many);
    
    for(i = 0; i < how_many; ++ i) {
        sid = fs_open_server("0.0.0.0");
        printf("Otwieram %d z %d, session_id %d\n", i, how_many, sid);
        
        fs_close_server(sid);
        printf("Zamykam %d z %d, session_id %d\n", i, how_many, sid);

    }
    
	return 0;
}
