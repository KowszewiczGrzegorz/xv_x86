
#include "webAPI.hh"

WebAPI::WebAPI(int portNumber, XV25* xv25) {
    m_xv25 = xv25;

    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0) {
        cerr << "ERROR opening socket" << endl;
        m_status = STATUS_ERROR;
        return;
    }
    
    bzero((char *) &m_serv_addr, sizeof(m_serv_addr));
    m_serv_addr.sin_family = AF_INET;
    m_serv_addr.sin_addr.s_addr = INADDR_ANY;
    m_serv_addr.sin_port = htons(portNumber);
    if (bind(m_sockfd, (struct sockaddr *) &m_serv_addr, sizeof(m_serv_addr)) < 0) {
        cerr << "ERROR on binding" << endl;
        m_status = STATUS_ERROR;
        return;
    }

    listen(m_sockfd, 5);
    m_clilen = sizeof(m_cli_addr);
    m_newsockfd = accept(m_sockfd, (struct sockaddr *) &m_cli_addr, &m_clilen);
    if (m_newsockfd < 0) {
        cerr << "ERROR on accept" << endl;
        m_status = STATUS_ERROR;
        return;
    }
}

WebAPI::~WebAPI() {
    if (m_newsockfd > 0)
        close(m_newsockfd);
    if (m_sockfd > 0)
        close(m_sockfd);
}

void WebAPI::run() {
    int n;
    string response;

    if (STATUS_ERROR == m_status)
        return;

    for (;;) {
        bzero(m_buffer, 256);
        n = read(m_newsockfd, m_buffer, 255);
        if (n < 0) {
            cerr << "ERROR reading from socket" << endl;
        } else {
            response = m_xv25->interpretCommand(m_buffer);
            
            if (response.size() > 0) {
                n = write(m_newsockfd, response.c_str(), response.size());
                if (n < 0) {
                    cerr << "ERROR writing to socket" << endl;
                }
            } else {
                cerr << "no response needed" << endl;
            }
        }
    }
}
