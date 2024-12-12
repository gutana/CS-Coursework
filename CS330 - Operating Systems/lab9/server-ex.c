/*****************************************************************
 Sockets Daemon Program

  This code was modified from Nigel Horspools, "The Berkeley
  Unix Environment".

  A daemon process is started on some host.  A socket is acquired
  for use, and it's number is displayed on the screen.  For clients
  to connect to the server, they muse use this socket number.
*****************************************************************/

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Use port number 0 so that we can dynamically assign an unused
 * port number. */
#define PORTNUM 0

/* Set up the location for the file to display when the daemon (okay,
 * server for you religious types) is contacted by the client. */
#define BBSD_FILE "./test.bbs.file"
/*"/nfs/net/share/ftp/pub/class/330/test.file" */

/* Display error message on stderr and then exit. */
#define OOPS(msg)    \
    {                \
        perror(msg); \
        exit(1);     \
    }

#define MAXLINE 512

int main()
{
    struct sockaddr_in saddr; /* socket information */

    int s;         /* socket return value */
    int socket_fd; /* socket descriptor returned from accept() */
    int num_char = MAXLINE;
    // char in_char[MAXLINE]; /* character for i/o */

    /*
     * Build up our network address. Notice how it is made of machine name + port.
     */

    /* Clear the data structure (saddr) to 0's. */
    memset(&saddr, 0, sizeof(saddr));

    /* Tell our socket to be of the internet family (AF_INET). */
    saddr.sin_family = AF_INET;

    /* Aquire the name of the current host system (the local machine). */
    char hostname[256]; /* host computer */
    gethostname(hostname, sizeof(hostname));
    printf("The hostname is: %s\n", hostname);

    /* Return misc. host information.  Store the results in the
     * hp (hostent) data structure.  */
    struct hostent *hp; /* host information */
    hp = gethostbyname(hostname);
    printf("The first hostaddress is: %s\n",
           inet_ntoa(*((struct in_addr *)(hp->h_addr_list[0]))));

    /* Copy the host address to the socket data structure. */
    memcpy(&saddr.sin_addr, hp->h_addr, hp->h_length);

    /* Convert the integer Port Number to the network-short standard
     * required for networking stuff. This accounts for byte order differences.*/
    saddr.sin_port = htons(PORTNUM);

    /*
     * Now that we have our data structure full of useful information,
     * open up the socket the way we want to use it.
     */
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
        OOPS("socket");

    /* Register our address with the system. */
    if (bind(s, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
        OOPS("bind");

    /* Display the port that has been assigned to us. */
    socklen_t slen; /* length socket address */
    slen = sizeof(saddr);
    getsockname(s, (struct sockaddr *)&saddr, &slen);
    printf("Socket assigned: %d\n", ntohs(saddr.sin_port));

    /* Tell socket to wait for input.  Queue length is 1. */
    if (listen(s, 1) != 0)
        OOPS("listen");

    /* Loop indefinately and wait for events. */
    for (;;)
    {
        /* Wait in the 'accept()' call for a client to make a connection. */
        socket_fd = accept(s, NULL, NULL);
        if (socket_fd == -1)
            OOPS("accept");
        printf("Connection established\n");
        fflush(stdout);

        // Read char from the socket
        char c;
        if (read(socket_fd, &c, 1) > 0)
        {

            printf("Client asked for option %c\n", c);
            fflush(stdout);

            // Fork

            if (fork()) // PARENT
            {
                wait(NULL);
            }
            else // CHILD
            {
                // In child, use dup2 so that stdout has the socket_fd on it instead
                if (dup2(socket_fd, fileno(stdout)) == -1)
                {
                    OOPS("setting stdout file descriptor to socket.");
                }
                // Use a switch statement to decide which statement to execute
                switch (c)
                {
                case '1':
                    execl("/bin/date", "", NULL);
                    break;
                case '2':
                    execl("/bin/env", "", NULL);
                    break;
                case '3':
                    execl("/bin/ip", "ip", "addr", "show", NULL);
                    break;
                }
                // Execute the command
                exit(0);
            }
        }
        else
        {
            printf("Something wrong with client.\n");
            fflush(stdout);
        }
        close(socket_fd);
    }

    return 0;
}
