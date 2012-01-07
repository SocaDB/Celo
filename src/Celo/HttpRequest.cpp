#include "HttpRequest.h"
#include "StringHelp.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

const int size_buff = 1024;

HttpRequest::HttpRequest( int fd ) : EventObj_WO( fd ), inp_cont( 0 ), url_rese( 0 ), content_length( 0 ) {
}

HttpRequest::~HttpRequest() {
    if ( url_rese )
        free( url_data );
}

void HttpRequest::inp() {
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff < 0 ) {
            if ( errno == EAGAIN )
                continue;
            inp_cont = 0;
            return;
        }

        // at the end ?
        if ( ruff == 0 ) {
            inp_cont = 0;
            return;
        }

        // parse
        inp( buff, buff + ruff );

        //
        if ( not inp_cont )
            return;
    }
}

bool HttpRequest::end() {
    return EventObj_WO::end() and not inp_cont;
}

void HttpRequest::req() {
    // not found
    error_404();
}

bool HttpRequest::send_file( const char *url ) {
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

void HttpRequest::send_head( const char *url ) {
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


void HttpRequest::inp( char *data, const char *end ) {
    // write( 0, data, end - data );
    if ( inp_cont )
        goto *inp_cont;

// ----------------- REQUEST TYPE -----------------
#include "HttpRequest_read_req.h"

// ----------------- URL -----------------
a_url_beg:
    ++data;
b_url_beg:
    if ( data >= end ) goto c_url_beg;
l_url_beg: // URL, first call
    url_data = data;
    while ( true ) {
        if ( *data == ' ' ) {
            url_size = data - url_data;
            *data = 0;
            goto e_url;
        }
        if ( ++data == end ) {
            const char *old = url_data;
            url_size = data - url_data;
            url_rese = 2 * size_buff;
            url_data = (char *)malloc( url_rese );
            memcpy( url_data, old, url_size );
            goto c_url_cnt;
        }
    }


l_url_cnt: // URL, cont
    while ( true ) {
        if ( url_size == url_rese ) {
            char *old = url_data;
            url_data = (char *)malloc( url_rese *= 2 );
            memcpy( url_data, old, url_size );
            free( old );
        }

        if ( *data == ' ' ) {
            url_data[ url_size ] = 0;
            goto e_url;
        }
        url_data[ url_size++ ] = *data;

        if ( ++data == end )
            goto c_url_cnt;
    }

e_url:
    if ( req_type == GET ) {
        inp_cont = 0;
        return req();
    }

// -------------- Header Fields --------------
#include "HttpRequest_header_fields.h"

// Content-Length
a_read_cl:
    ++data;
b_read_cl:
    if ( data >= end ) goto c_read_cl;
l_read_cl:
    if ( *data < '0' or *data > '9' ) goto l_header_fields_;
    content_length = 10 * content_length + ( *data - '0' );
    goto a_read_cl;

// ----------------- ERRORS ------------------

// c_...
#include "HttpRequest_conts.h"

// e_...
#define ERR( NUM, MSG ) e_##NUM: inp_cont = 0; return error_##NUM();
#include "ErrorCodes.h"
#undef ERR

l_user:
    return;
}

#define ERR( NUM, MSG ) \
    void HttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\n\n"; \
        send( s, sizeof( s ) - 1 ); \
    }

#include "ErrorCodes.h"

#undef ERR
