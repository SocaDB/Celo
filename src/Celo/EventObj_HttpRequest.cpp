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


#include "EventObj_HttpRequest.h"

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <string>

EventObj_HttpRequest::EventObj_HttpRequest( int fd ) : EventObj_WO( fd ) {
}

EventObj_HttpRequest::EventObj_HttpRequest( VtableOnly vo ) : EventObj_WO( vo ) {
}

bool EventObj_HttpRequest::send_file( const char *url ) {
    // open
    int src = open( url, O_RDONLY );
    if ( src < 0 ) {
        if ( not *url )
            return send_file( "index.html" );
        return false;
    }

    // stat
    struct stat stat_buf;
    if ( fstat( src, &stat_buf ) ) {
        close( src );
        return false;
    }

    // directory ?
    if ( S_ISDIR( stat_buf.st_mode ) ) {
        std::string nrl = url;
        nrl += "/index.html";
        return send_file( nrl.c_str() );
    }

    //
    send_head( url );
    send_fid( src, 0, stat_buf.st_size );
    return true;
}

void EventObj_HttpRequest::send_head( const char *url ) {
    const char *end = url;
    while ( *end )
        ++end;

    while ( end > url and *(--end) != '.' );

    #define SH( M, R ) \
        if ( strcmp( end, M ) == 0 ) { \
            const char s[] = "HTTP/1.0 200 OK\nContent-Type: " R "\n\n"; \
            send_cst( s, sizeof( s ) - 1, false ); \
            return; \
        }    
    #include "MimeTypes.h"
    #undef SH

    // default  -> plain text
#ifdef SCEEN
    const char s[] = "";
#else
    const char s[] = "HTTP/1.0 200 OK\nContent-Type: text/plain\n\n";
#endif
    send_cst( s, sizeof( s ) - 1, false );
}

//
#define ERR( NUM, MSG ) \
    void EventObj_HttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\nContent-Type: text/plain\nContent-Length: 50\nStatus: " #NUM "\n\nERROR " #NUM ": " MSG "                                                  "; \
        send_cst( s, sizeof( s ) - 1, true ); \
    }
#include "ErrorCodes.h"
#undef ERR

