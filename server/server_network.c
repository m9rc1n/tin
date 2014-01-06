#include "server_network.h"

int s_open_server(IncomingRequest *inc_request)
{
	printf ("Opening session with: %d\n", server_handler);
    FsResponse response;
    response.response_data.open_server_s.server_handler = server_handler++;
    response.answer = ACCEPTED;
    
    printf("%s\n", inet_ntoa(inc_request->client_addr.sin_addr));

	return sendto(sockd, &response, sizeof (FsResponse), 0, (struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_close_server(IncomingRequest *inc_request)
{
	printf("Closing session with: %d\n", inc_request->request.request_data.close_server_c.server_handler);
    FsResponse response;
    response.answer = CLOSED;
   
	return sendto(sockd, &response, sizeof (FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}
