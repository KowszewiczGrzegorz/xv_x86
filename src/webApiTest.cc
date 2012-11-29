#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    char *tmp;
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        error("ERROR opening socket");
    if (NULL == (server = gethostbyname(argv[1]))) {
        close(sockfd);
        error("ERROR, no such host\n");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    for (;;) {
        printf("Please enter the command: ");
        bzero(buffer, 256);
        tmp = fgets(buffer, 255, stdin);
        
        if (48 == tmp[0])
            break;

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            close(sockfd);
            error("ERROR writing to socket");
        }

        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            close(sockfd);
            error("ERROR reading from socket");
        } else {
            cerr << "Response \"" << buffer << "\"" << endl;
        }
    }

    printf("%s\n", buffer);
    close(sockfd);
    return 0;

}
