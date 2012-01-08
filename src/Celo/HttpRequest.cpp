#include "HttpRequest.h"
#include "StringHelp.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

const int size_buff = 1024;

HttpRequest::HttpRequest( int fd ) : EventObj_WO( fd ), inp_cont( 0 ), content_length( 0 ) {
}

HttpRequest::~HttpRequest() {
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

void HttpRequest::req_GET() {
    // not found
    error_404();
}

void HttpRequest::req_PUT( char *beg, int len ) {
    // not found
    error_404();
}

void HttpRequest::req_POST( char *beg, int len ) {
    write( 0, beg, len );
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

    #include "HttpRequest_gen.h"

    // e_...
    #define ERR( NUM, MSG ) e_##NUM: inp_cont = 0; return error_##NUM();
    #include "ErrorCodes.h"
    #undef ERR
}

#define ERR( NUM, MSG ) \
    void HttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\n\n"; \
        send( s, sizeof( s ) - 1 ); \
    }

#include "ErrorCodes.h"

#undef ERR
