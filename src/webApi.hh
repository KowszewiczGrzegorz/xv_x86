
#ifndef __WEBAPI_HH__
#define __WEBAPI_HH__

#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "xv25.hh"

class WebApi
{
public:
    WebApi(XV25*, int);
    ~WebApi();

    void run(bool*);
    status_t getStatus() { return m_status; };

private:
    XV25* m_xv25;
    int m_sockfd, m_newsockfd;
    socklen_t m_clilen;
    char m_buffer[256];
    struct sockaddr_in m_serv_addr, m_cli_addr;
    status_t m_status;
};

#endif /* __WEBAPI_HH__ */
