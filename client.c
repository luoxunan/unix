/**
*Function:set up a TCP/IPv4 client,then send words from keyboard input,
*             and receive the words what the server reply. 
*             
*Parameters:IP address;   port number
*return:
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <stdarg.h>



int main(int argc, char *argv[])
{
    int fd, connectint, bytes, port;
    struct sockaddr_in abc;
    char buf[100];                           /* read/write buffer area */
    memset((struct sockaddr_in *)&abc, 0 , sizeof(abc));    /* initialize struct abc */

    /**
      *creat a socket with IPv4 and TCP type 
      */
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)         
       {
        printf("socket defeat\n");
        exit(1);
       }

    abc.sin_family = AF_INET;             /* set address family */
    port = atol(argv[2]);                    /* change port character to int */
    abc.sin_port = htons(port);           /* set port number */
    if ((inet_pton(AF_INET, argv[1], &abc.sin_addr)) <= 0)     /* set IP address */
       {
        printf("IP change wrong\n");
        exit(1);
       }
       
     /**
       *connect server
       */
    if ((connectint = connect(fd, (struct sockaddr_in *)&abc,(socklen_t)sizeof(abc))) < 0)
       {
        printf("connect defeat\n");
        exit(1);
       }
    else
       {
        printf("connect succeed\n");
       }
    printf("connectint= %d\n",connectint);

    /**
      *main loop
      */
    while(1)
       {
        printf("please enter some words:\n");
        gets(buf);
        write(fd, buf , 100);         /* send words */
        memset(buf, 0 ,100);
        read(fd, buf, 100);           /* receive words */
        printf("reply: %s\n", buf); 
        memset(buf, 0 , 100);
       }
}

