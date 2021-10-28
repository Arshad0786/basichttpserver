#include <stdio.h>    /*  standard I/O  */
#include <stdlib.h>   /*  standard library */
#include <string.h>   /*  string library */
#include <winsock2.h> /*  socket definitions */

#define MYPORT 1024
#define BACKLOG 5
int main(int argc, char *argv[])
{
    int sockfd, new_fd;            // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;    // my address information
    struct sockaddr_in their_addr; // connector’s address information
    int numberofbytes;             //mark the position of the end of received string
    int sin_size;
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa); // winsock start up
    char rcvbuffer[3000];
    char sndbuffer[3000];
    char *sndbufferpointer = sndbuffer;
    char requestedpath[20];
    char *hello = "HTTP/1.1 200 OK\n\nHello World!\n";

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("can't create socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;         // host byte order (AF_INET -> IPV4)
    my_addr.sin_port = htons(MYPORT);     // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(my_addr.sin_zero), 0, 8);    // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    while (1)
    { // main accept() loop

        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }

        if ((numberofbytes = recv(new_fd, rcvbuffer, 3000, 0)) == -1)
        {
            perror("receive");
            exit(1);
        }
        else
        {
            rcvbuffer[numberofbytes] = '\0'; //mark the end of string
            printf("%s", rcvbuffer);

            /* parse file name from http header */

            char *buffer = rcvbuffer;
            char *startofpath = strchr(buffer, '/') + 1;
            int i = 0;
            while (*(startofpath + i) != ' ')
            {
                requestedpath[i] = *(startofpath + i);
                i++;
            }
            requestedpath[i] = '\0';
            printf("Request PATH : %s\n", requestedpath);
            printf("length : %d\n", strlen(requestedpath));
        }

        strcat(sndbuffer, "HTTP/1.1 200 OK\n\n");

        if (strcmp(requestedpath, "index.html") == 0 || strcmp(requestedpath, "page1.html") == 0 || !strlen(requestedpath))
        {
            FILE *fp;
            fp = fopen("page1.html", "r");
            char buffer[100];
            while (fgets(buffer, sizeof(buffer), fp) != NULL)
            {
                strcat(sndbuffer, buffer);
            }
            printf("sendbuffer : \n%s", sndbufferpointer);
            fclose(fp);
        }
        else if (strcmp(requestedpath, "page2.html") == 0){
            FILE *fp;
            fp = fopen("page2.html", "r");
            char buffer[100];
            while (fgets(buffer, sizeof(buffer), fp) != NULL)
            {
                strcat(sndbuffer, buffer);
            }
            printf("sendbuffer : \n%s", sndbufferpointer);
            fclose(fp);
        }
        else{
            strcat(sndbuffer,"404 not found.");
            printf("sendbuffer : \n%s", sndbufferpointer);
        }
        printf("\n\n");
        printf("---------------------------------------------------------------\n");
        int count;
        if (send(new_fd, sndbufferpointer, strlen(sndbufferpointer), 0) == -1)
            perror("send");
        
        memset(sndbuffer,0,strlen(sndbuffer));
        memset(rcvbuffer,0,sizeof(rcvbuffer));
        closesocket(new_fd);
    } // end of while
    WSACleanup();
    return 0;
}
