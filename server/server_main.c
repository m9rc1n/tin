// server
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "../protocol.h"

#include "server_io.h"
#include "server_network.h"
#include "server_reset.h"
#include "server_session.h"
#include "server_synchroniser.h"
#include "server_thread.h"
#include "server_zombie_collector.h"

int status, sockd;
struct sockaddr_in my_name;
int server_handler = 0;

void exit_handler(int signo)
{
    printf("\nClosing socket\n");
    close(sockd);
    printf("Closing program\n");
    exit(0);
}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		fprintf(stderr, "Usage: %s port_number\n", argv[0]);
		exit(1);
	}

	logopen();
	
    signal (SIGINT, exit_handler);

	// create a socket
	sockd = socket(AF_INET, SOCK_DGRAM, 0);

    struct timeval time_val;

    time_val.tv_sec = WAIT_TO_STOP_RCV;
    time_val.tv_usec = 0;

	if (sockd == -1) {
		perror("Socket creation error");
		exit(1);
	}

	// server address
	my_name.sin_family = AF_INET;
	my_name.sin_addr.s_addr = INADDR_ANY;
	my_name.sin_port = htons (atoi (argv[1]));
	status = bind(sockd, (struct sockaddr*) &my_name, sizeof (my_name));

    if(status != 0) {
        perror("Socket binding error");
        exit(1);
    };

	synchroniser_init();
    session_init();
    zombie_collector_init();
    

	while(1)
	{
        pthread_t handlers_thread;

        struct IncomingRequest *request = (struct IncomingRequest *) calloc(1, sizeof(struct IncomingRequest));
        request->client_addr_len = sizeof(struct sockaddr_in);

		recvfrom(sockd, (char*) &(request->request), MAX_BUF, 0, (struct sockaddr *) &(request->client_addr), &(request->client_addr_len));

        pthread_create(&handlers_thread, NULL, server_thread_function, (void *) request);
        pthread_detach(handlers_thread);
	}

	close(sockd);

    session_shutdown();
	synchroniser_shutdown();
    zombie_collector_shutdown();

    logclose();
    
    return 0;
}
