#include <stdio.h>            /*  standard I/O  */
#include <stdlib.h>           /*  standard library */
#include <string.h>           /*  string library */
#include <winsock2.h>       /*  socket definitions */
#include <sys/types.h>        /*  socket types       */


#define PORT 80 // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once
int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    char rcvbuff[3000];
    char sndbuff[3000];
    struct sockaddr_in server_addr; // connector’s address information

   WSADATA wsa;
   WSAStartup(MAKEWORD(2,2),&wsa);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror("socket");
       exit(1);
    }

    server_addr.sin_family = AF_INET; // host byte order
    server_addr.sin_port = htons(PORT); // short, network byte order
    server_addr.sin_addr.s_addr = inet_addr("140.118.115.191");
    memset(&(server_addr.sin_zero), 0, 8); // zero the rest of the struct

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
       perror("connect");
       exit(1);
    }

    if ((send(sockfd, "GET / HTTP/1.1\n", MAXDATASIZE-1, 0)) == -1) {
       perror("send");
       exit(1);
    }

    if ((numbytes=recv(sockfd, rcvbuff, 3000, 0)) == -1) {
       perror("recv");
       exit(1);
    }
   rcvbuff[numbytes] = '\0';
    printf("Received: %s",rcvbuff);

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

