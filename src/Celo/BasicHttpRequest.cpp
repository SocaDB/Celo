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


#include "BasicHttpRequest.h"
#include "StringHelp.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

const int size_buff = 1024;

BasicHttpRequest::BasicHttpRequest( int fd ) : EventObj_WO( fd ), inp_cont( 0 ), content_length( 0 ) {
}

BasicHttpRequest::~BasicHttpRequest() {
}

void BasicHttpRequest::req_GET() {
    // not found
    error_404();
}

void BasicHttpRequest::req_PUT( char *beg, ST len ) {
    // forbidden
    error_403();
}

void BasicHttpRequest::req_POST( char *beg, ST len ) {
    // forbidden
    error_403();
}

bool BasicHttpRequest::send_file( const char *url ) {
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
    send( src, 0, stat_buf.st_size );
    return true;
}

void BasicHttpRequest::send_head( const char *url ) {
    const char *end = url;
    while ( *end )
        ++end;

    while ( end > url and *(--end) != '.' );

    #define SH( M, R ) \
        if ( strcmp( end, M ) == 0 ) { \
            const char s[] = "HTTP/1.0 200 OK\nContent-Type: " R "\n\n"; \
            send( s, sizeof( s ) - 1, false ); \
            return; \
        }

    SH( ".html", "text/html" );
    SH( ".css" , "text/css"  );
    SH( ".png" , "image/png" );
    SH( ".jpg" , "image/jpg" );
    SH( ".jpeg", "image/jpg" );
    SH( ".js"  , "text/javascript" );

    // default  -> plain text
    const char s[] = "HTTP/1.0 200 OK\nContent-Type: text/plain\n\n";
    send( s, sizeof( s ) - 1, false );
}


bool BasicHttpRequest::inp( char *data, const char *end ) {
    #include "BasicHttpRequest_gen.h"
}

#define ERR( NUM, MSG ) \
    void BasicHttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\n\n"; \
        send( s, sizeof( s ) - 1 ); \
    }

#include "ErrorCodes.h"

#undef ERR
