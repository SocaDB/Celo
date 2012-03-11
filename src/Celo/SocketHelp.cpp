#include "SocketHelp.h"

#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
// #include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>

int listening_socket( const char *port ) {
    // stolen from https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/
    addrinfo hints;
    memset( &hints, 0, sizeof( addrinfo ) );
    hints.ai_family   = AF_UNSPEC;   /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags    = AI_PASSIVE;  /* All interfaces */

    addrinfo *result;
    if ( int s = getaddrinfo( NULL, port, &hints, &result ) ) {
        fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( s ) );
        return -1;
    }

    for ( addrinfo *rp = result; rp; rp = rp->ai_next ) {
        // socket ok ?
        int listen_fd = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );
        if ( listen_fd < 0 )
            continue;

        // no "Address already in use" error message
        int yes = 1;
        if ( setsockopt( listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) == -1 )
            perror( "setsockopt" );

        // TCP_CORK
        //if ( setsockopt( listen_fd, SOL_TCP, TCP_CORK, &yes, sizeof( yes ) ) == -1 )
        //    perror( "setsockopt" );

        // bind ok ?
        if ( bind( listen_fd, rp->ai_addr, rp->ai_addrlen ) == 0 ) {
            freeaddrinfo( result );

            if ( set_non_blocking( listen_fd ) == -1 ) {
                close( listen_fd );
                return -1;
            }

            if ( listen( listen_fd, SOMAXCONN ) == -1 ) {
                perror( "listen" );
                close( listen_fd );
                return -1;
            }

            return listen_fd;
        }

        close( listen_fd );
    }

    fprintf( stderr, "Could not bind\n" );
    freeaddrinfo( result );
    return -1;
}

int accepting_socket( int listen_fd ) {
    // accept
    sockaddr_in local;
    socklen_t addrlen = sizeof( sockaddr_in );
    int conn_sock = accept( listen_fd, (struct sockaddr *)&local, &addrlen );
    if ( conn_sock == -1 ) {
        //perror( "accept" );
        return -1;
    }

    // socket parameters
    if ( set_non_blocking( conn_sock ) ) {
        close( conn_sock );
        return -1;
    }

    return conn_sock;
}

int set_non_blocking( int fd ) {
    int flags = fcntl( fd, F_GETFL );
    if ( flags < 0 )
        return flags;
    flags |= O_NONBLOCK;
    if ( fcntl( fd, F_SETFL, flags ) < 0 )
        return -1;
    return 0;
}

