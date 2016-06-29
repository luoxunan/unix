/**
*Function:set up a TCP/IPv4 server,then receive words from client,
*             and reply the same words to client.It can connect 5 client
*             at the same time.
*Parameters:port number
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
#include <sys/select.h>

/**
  *the number of listen
  */
#define LISTEN_NUM 5 

int main(int argc,char *argv[])
{
    int fd, fd1, bindint, listenint, pid, set[6], i, j, max, port, bytes, num, count;
    struct sockaddr_in abc, def;        
    char buf[100];                 /* read/write buffer area */
    fd_set fds;                    
    u_long ul = 0;
    memset((struct sockaddr_in *)&abc, 0, sizeof(abc));     /* initialize struct abc */
    memset(set, 0, sizeof(set));           /* clear all bits of fds */
    port = atol(argv[1]);                /* change port character to int */
    printf("start\n");
     
    /* *
      *creat a socket with IPv4 and TCP type
      */
    if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)       
       {
        printf("creat socket defeat\n");
        exit(1);
       }

    abc.sin_family = AF_INET;                              /* set address family */
    abc.sin_addr.s_addr = htonl(INADDR_ANY);       /* INADDR_ANY means the socket can be linker with all address  */
    abc.sin_port = htons(port);             /* change little endian to big endian */

    /* *
      *bind struct abc and the socket together
      */
    if ((bindint = bind(fd,  (const struct sockaddr_in * )&abc, sizeof(abc))) < 0)  
       {
        printf("bind defeat\n");
        exit(1);
       }

    /**
      *listen the socket
      */
    if ((listenint = listen(fd, LISTEN_NUM)) < 0)  
       {
        printf("listen defeat\n");
        exit(1);
       }

    set[0] = fd;          
    max = 0;
    while (1)                      /* main loop */
       {
        FD_ZERO(&fds);           /* clear fds */

    /**
      *when a client connects this server or this server receives some words,  /
      *the socket will be readable,so we can use select( ) to wait these change
      */
        for (i=0; i<6; i++)          /* this cycle adds all member of set[] to fds */
           {
            if (set[i])  
               {
                FD_SET(set[i], &fds);
                max = set[i] > max ? set[i] : max;
                j = i;
               }
           }
        printf("waiting for connection and reception...\n");    
        num = select((max+1), &fds, NULL, NULL, NULL);     /*wait for the member's change in fds */
        count = 0;
        if (FD_ISSET(fd, &fds))                /* test if a client connects this server */
           {
            if ((fd1 = accept(fd, 0, 0)) < 0 )        /* accept the client */ 
               {
                printf("accept defeat\n");
               }  
            else 
               {
                printf("accept client %d succeed\n", fd1);
               }
            set[j + 1] = fd1;                   /* add the new client to fds */
            continue;
           }
        for (i = 1; i<=j; i++)
           {
            if (FD_ISSET(set[i], &fds))       /*test if any socket has receive any date*/
               {
                memset(buf, 0, 100);
                if ((bytes = (read(set[i], buf, 100))) <= 0)     /* read date received*/

     /**
       *if read defeat,the connect may had be breaked,/
       *then we should close this connect to protect the server
       */
                   {
                    printf("bytes = %d\n",bytes);
                    printf("read %d defeat\n",set[i]);
                    close(set[i]);                                  /* close a connect */
                    printf("client %d has closed\n",set[i]);
                    set[i] = 0;
                    continue;
                   }
                printf("from client %d: %s\n",set[i] ,buf);    /* print the words we received */
                write(set[i], buf, 100);            /* send the words to client */
                memset(buf, 0 ,100);            /* clear buffer area */
                if((++count) == num)
                   {
                    break;
                   }
               }
           }
       }
}


