#include "HttpRequest.h"
#include "StringHelp.h"
#include <unistd.h>

HttpRequest::HttpRequest( int fd ) : EventObj( fd ), cur_proc( READ_REQ_TYPE ) {
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
        if ( ruff <= 0 )
            return true;

        char *data = buff, *end = buff + size;
        while ( cur_proc = inp( data, end ) and data < end );
    }
}

bool HttpRequest::out() {
    PRINT( "out" );
    return false;
}

void HttpRequest::err() {
    PRINT( "err" );
}

void HttpRequest::hup() {
    PRINT( "hup" );
}

int HttpRequest::inp( char *&data, const char *end ) {
    switch ( cur_proc ) {
    case READ_REQ_TYPE_B: goto label_read_B;
    }

label_read_B:

    // ""
    if ( *data == 'P' ) goto label_read_P;
    if ( *data != 'B' ) return error_400();
    if ( ++data >= end ) return READ_REQ_TYPE_B;
    // "B"
label_read_B:
label_read_P:


    switch ( cur_proc ) {
    case READ_REQ_TYPE:
        if ( *data == 'B' ) return READ_REQ_TYPE_B;
        if ( *data == 'P' ) return READ_REQ_TYPE_P;
    case READ_REQ_TYPE_B:
        if ( *data == 'B' ) return READ_REQ_TYPE_B;
    default:
        return true;
    }
}

#define ERR( NUM, MSG ) \
    void HttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\n\n"; \
        send( s, sizeof( s ) - 1 ); \
        return READ_REQ_BEG_ERR + NUM; \
    }

#include "ErrorCodes.h"

#undef ERR
