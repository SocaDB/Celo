#include "HttpRequest.h"
#include "StringHelp.h"
#include <unistd.h>
#include <errno.h>

HttpRequest::HttpRequest( int fd ) : EventObj( fd ), cur_proc( 0 ) {
}

HttpRequest::~HttpRequest() {
}

bool HttpRequest::inp() {
    //write( 0, buff, cnt );
    //PRINT( "inp" );
    const int size_buff = 1024;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );        
        if ( ruff < 0 ) {
            if ( errno == EAGAIN )
                continue;
            return true;
        }
        if ( ruff == 0 )
            return true;

        if ( inp( buff, buff + ruff ) )
            return true;
    }
}

bool HttpRequest::out() {
    return false;
}

void HttpRequest::err() {
}

void HttpRequest::hup() {
}

#define LAB( N ) \
    b_##N: \
        if ( ++data >= end ) { cur_proc = N; return false; } \
    l_##N


bool HttpRequest::inp( char *data, const char *end ) {
    switch ( cur_proc ) {
    case 0: goto l_0;
    }

l_0: // ""
    if ( *data == 'P' ) goto b_10;
    if ( *data != 'G' ) return error_400();
LAB( 1 ): // G
    if ( *data != 'E' ) return error_400();
LAB( 2 ): // GE
    if ( *data != 'T' ) return error_400();
LAB( 3 ): // GET
    if ( *data != ' ' ) return error_400();
    req_type = GET;

LAB( 4 ): // url (beg)
    url = data;
    while ( true ) {
        if ( ++data == end ) {
            break;
        }
        if ( *data == ' ' ) {
            *data = 0;
            PRINT( url );
            break;
        }
    }

    goto b_100;

LAB( 10 ): // P
    if ( *data != 'O' ) return error_400();
    PRINT( "post" );

b_100:


    return true;
}

#define ERR( NUM, MSG ) \
    bool HttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\n\n"; \
        send( s, sizeof( s ) - 1 ); \
        return true; \
    }

#include "ErrorCodes.h"

#undef ERR
