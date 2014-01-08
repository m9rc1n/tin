#include "server_network.h"
#include "server_session.h"

int s_open_server(IncomingRequest *inc_request)
{
    int session_id;
    
    session_id = session_create();
    
	printf ("Opening session with: %d\n", session_id);
    FsResponse response;
    response.response_data.open_server_s.server_handler = session_id;
    response.answer = ACCEPTED;

	return sendto(sockd, &response, sizeof (FsResponse), 0, (struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}

int s_close_server(IncomingRequest *inc_request)
{
    int session_id;
    
    session_id = inc_request->request.request_data.close_server_c.server_handler;
    session_close(session_id);
    
	printf("Closing session with: %d\n", session_id);
    FsResponse response;
    response.answer = CLOSED;
   
	return sendto(sockd, &response, sizeof (FsResponse), 0,(struct sockaddr*) &(inc_request->client_addr), inc_request->client_addr_len);
}
