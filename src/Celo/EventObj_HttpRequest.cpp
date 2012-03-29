#include "EventObj_HttpRequest.h"

#include <sys/stat.h>
#include <unistd.h>
//#include <stdlib.h>
#include <string.h>
#include <string>
#include <fcntl.h>
//#include <errno.h>

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
    const char s[] = "HTTP/1.0 200 OK\nContent-Type: text/plain\n\n";
    send_cst( s, sizeof( s ) - 1, false );
}

#define ERR( NUM, MSG ) \
    void EventObj_HttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\n\n"; \
        send_cst( s, sizeof( s ) - 1 ); \
    }
#include "ErrorCodes.h"
#undef ERR

