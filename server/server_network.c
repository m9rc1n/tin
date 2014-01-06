#include "server_network.h"

int s_open_server(int sockd, struct IncomingRequest *inc_request, int server_handler)
{
    FsResponse response;
    response.response_data.server_handler = server_handler;
    response.answer = ACCEPTED;
    
    printf("%s\n", inet_ntoa(inc_request->client_addr.sin_addr));
    
	printf ("Opening session with: %d\n", server_handler);
	return sendto(sockd, &response, sizeof (FsResponse), 0, (struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_close_server(int sockd, FsRequest *fs_request, struct sockaddr_in cli_name, int addrlen, int server_handler)
{
    FsResponse response;
    response.answer = CLOSED;
    	
	printf("Closing session with: %d\n", fs_request->request_data.server_handler);
	return sendto(sockd, &response, sizeof (FsResponse), 0, (struct sockaddr*) &cli_name, sizeof (cli_name));
}
