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
    case READ_REQ_TYPE:     goto label_read;
    case READ_REQ_TYPE_B:   goto label_read_;
    case READ_REQ_TYPE_BE:
    case READ_REQ_TYPE_BEG:
    case READ_REQ_TYPE_P:
    case READ_REQ_TYPE_PO:
    case READ_REQ_TYPE_POS:
    case READ_REQ_TYPE_POST:
    case READ_REQ_TYPE_PU:
    case READ_REQ_TYPE_PUT:
    case READ_REQ_TYPE_PUT_:

    case READ_REQ_TYPE_URL:

    default:
        PRINT( (int)cur_proc - READ_REQ_BEG_ERR );
        return 0;
    }

label_read: // ""
    if ( *data == 'P' ) goto label_read_P;

    if ( *data != 'B' ) return error_400();
    if ( ++data >= end ) return READ_REQ_TYPE_B;

label_read_B: // "B"
    if ( *data != 'E' ) return error_400();
    if ( ++data >= end ) return READ_REQ_TYPE_BE;
label_read_BE: // "BE"
    if ( *data != 'G' ) return error_400();
    if ( ++data >= end ) return READ_REQ_TYPE_BEG;
label_read_BEG: // "BEG"
    if ( *data != ' ' ) return error_400();
    req_type = GET;
    if ( ++data >= end ) return READ_REQ_TYPE_URL;
    goto label_read_URL;

label_read_P: // "P"
    if ( *data == 'U' ) goto label_read_PU;

    if ( *data != 'O' ) return error_400();
    if ( ++data >= end ) return READ_REQ_TYPE_PO;
label_read_PO: // "PO"
    if ( *data != 'S' ) return error_400();
    if ( ++data >= end ) return READ_REQ_TYPE_POS;
label_read_POS: // "POS"
    if ( *data != 'T' ) return error_400();
    if ( ++data >= end ) return READ_REQ_TYPE_POST;
label_read_POST: // "POST"
    if ( *data != ' ' ) return error_400();
    req_type = POST;
    if ( ++data >= end ) return READ_REQ_TYPE_URL;
    goto label_read_URL;

label_read_PU: // "PU"
    if ( *data != 'T' ) return error_400();
    if ( ++data >= end ) return READ_REQ_TYPE_PUT;
label_read_PUT: // "PUT"
    if ( *data != ' ' ) return error_400();
    req_type = PUT;
    if ( ++data >= end ) return READ_REQ_TYPE_URL;
    goto label_read_URL;

label_read_URL:
    PRINT( *data );
    return READ_REQ_END;
}

#define ERR( NUM, MSG ) \
    int HttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\n\n"; \
        send( s, sizeof( s ) - 1 ); \
        return READ_REQ_BEG_ERR + NUM; \
    }

#include "ErrorCodes.h"

#undef ERR
