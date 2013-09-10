/*
 Copyright 2012 Structure Computation  www.structure-computation.com
 Copyright 2012 Hugo Leclerc

 This file is part of Celo.

 Celo is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Celo is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with Celo. If not, see <http://www.gnu.org/licenses/>.
*/


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
    if ( not port )
        return -1;

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

int accepting_socket( int listen_fd, int non_blocking ) {
    // accept
    sockaddr_in local;
    socklen_t addrlen = sizeof( sockaddr_in );
    int conn_sock = accept( listen_fd, (struct sockaddr *)&local, &addrlen );
    if ( conn_sock == -1 ) {
        //perror( "accept" );
        return -1;
    }

    // socket parameters
    if ( non_blocking ) {
        if ( set_non_blocking( conn_sock ) ) {
            close( conn_sock );
            return -1;
        }
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

