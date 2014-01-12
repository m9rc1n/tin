// server
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

int main(int argc, char* argv[]) {

	if (argc < 2) {
		fprintf(stderr, "Usage: %s port_number\n", argv[0]);
		exit(1);
	}


	// create a socket
	sockd = socket(AF_INET, SOCK_DGRAM, 0);

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
        // @todo sprawdzić, czy memleaka tu nie ma!
	}

	close(sockd);

    session_shutdown();
	synchroniser_shutdown();
    zombie_collector_shutdown();
	return 0;
}
