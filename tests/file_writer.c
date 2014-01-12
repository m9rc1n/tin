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
    int sid, fd, port;

    printf("Podaj port serwera: ");
    scanf("%d", &port);

    sid = fs_open_server("0.0.0.0", port);

    printf("Otwieram session_id %d\n", sid);

    FILE* send_file = fopen("a.x", "r+");

    if (send_file == NULL) perror("Cannot open the file");

    fseek(send_file, 0, SEEK_END);
    size_t file_size = ftell(send_file);
    fseek(send_file, 0, SEEK_SET);
    char* buf = (char*) calloc(file_size, sizeof(char));
    fread (buf, sizeof(char), file_size, send_file);
    fd = fs_open(sid,"b.x", "w");
    fs_write(sid, fd, buf, file_size);
    fs_close_server(sid);

    printf("Zamykam session_id %d\n", sid);

    return 0;
}
