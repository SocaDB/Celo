#include "HttpRequest.h"
#include "StringHelp.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

const int size_buff = 1024;

HttpRequest::HttpRequest( int fd ) : EventObj_WO( fd ), inp_cont( 0 ), url_rese( 0 ) {
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


// ----------------- REQ -----------------
l_: //
    // we have at least 4 bytes ?
    if ( end - data >= 4 ) {
        int val = *reinterpret_cast<const int *>( data );

        #if __BYTE_ORDER == __LITTLE_ENDIAN
        if ( val == 0x20544547 ) { data += 4; req_type = GET; goto   bur_; } // "GET "
        if ( val == 0x54534F50 ) { data += 4;                 goto b_POST; } // "POST"
        if ( val == 0x20545550 ) { data += 4; req_type = PUT; goto   bur_; } // "PUT "
        #elif __BYTE_ORDER == __BIG_ENDIAN
        if ( val == 0x47455420 ) { data += 4; req_type = GET; goto bur_; } // "GET "
        if ( val == 0x504F5354 ) { data += 4; goto b_POST; } // "POST"
        if ( val == 0x50555420 ) { data += 4; req_type = PUT; goto bur_; } // "PUT "
        #else
        Unknown endianness
        #endif
    }

    switch ( *data ) {
    case 'P': goto b_P;
    case 'D': goto b_D;
    case 'O': goto b_O;
    case 'H': goto b_H;
    case 'T': goto b_T;
    case 'C': goto b_C;
    case 'G': goto b_G;
    default: goto e_400;
    }

b_G:
    if ( ++data >= end ) goto c_G;
l_G:
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_GE;
l_GE:
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_GET;
l_GET:
    if ( *data != ' ' ) goto e_400;
    req_type = GET;
    goto burl;

b_4:
    if ( ++data >= end ) goto c_4;
l_4: // P
    if ( *data == 'U' ) goto b_8;
    if ( *data != 'O' ) goto e_400;
    if ( ++data >= end ) goto c_5;
l_5: // PO
    if ( *data != 'S' ) goto e_400;
    if ( ++data >= end ) goto c_6;
l_6: // POS
    if ( *data != 'T' ) goto e_400;
    ++data;
b_POST:
    if ( data >= end ) goto c_7;
l_7: // POST
    if ( *data != ' ' ) goto e_400;
    req_type = POST;
    goto burl;

b_8:
    if ( ++data >= end ) goto c_8;
l_8: // PU
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_9;
l_9: // PUT
    if ( *data != ' ' ) goto e_400;
    req_type = PUT;
    goto burl;


b_10:
    if ( ++data >= end ) goto c_10;
l_10: // D
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_11;
l_11: // DE
    if ( *data != 'L' ) goto e_400;
    if ( ++data >= end ) goto c_12;
l_12: // DEL
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_13;
l_13: // DELE
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_14;
l_14: // DELET
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_15;
l_15: // DELETE
    if ( *data != ' ' ) goto e_400;
    req_type = DELETE;
    goto burl;


b_16:
    if ( ++data >= end ) goto c_16;
l_16: // O
    if ( *data != 'P' ) goto e_400;
    if ( ++data >= end ) goto c_17;
l_17: // OP
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_18;
l_18: // OPT
    if ( *data != 'I' ) goto e_400;
    if ( ++data >= end ) goto c_19;
l_19: // OPTI
    if ( *data != 'O' ) goto e_400;
    if ( ++data >= end ) goto c_20;
l_20: // OPTIO
    if ( *data != 'N' ) goto e_400;
    if ( ++data >= end ) goto c_21;
l_21: // OPTION
    if ( *data != 'S' ) goto e_400;
    if ( ++data >= end ) goto c_22;
l_22: // OPTIONS
    if ( *data != ' ' ) goto e_400;
    req_type = OPTIONS;
    goto burl;

b_23:
    if ( ++data >= end ) goto c_23;
l_23: // H
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_24;
l_24: // HE
    if ( *data != 'A' ) goto e_400;
    if ( ++data >= end ) goto c_25;
l_25: // HEA
    if ( *data != 'D' ) goto e_400;
    if ( ++data >= end ) goto c_26;
l_26: // HEAD
    if ( *data != ' ' ) goto e_400;
    req_type = HEAD;
    goto burl;



b_27:
    if ( ++data >= end ) goto c_27;
l_27: // T
    if ( *data != 'R' ) goto e_400;
    if ( ++data >= end ) goto c_28;
l_28: // TR
    if ( *data != 'A' ) goto e_400;
    if ( ++data >= end ) goto c_29;
l_29: // TRA
    if ( *data != 'C' ) goto e_400;
    if ( ++data >= end ) goto c_30;
l_30: // TRAC
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_31;
l_31: // TRACE
    if ( *data != ' ' ) goto e_400;
    req_type = TRACE;
    goto burl;


b_32:
    if ( ++data >= end ) goto c_32;
l_32: // C
    if ( *data != 'O' ) goto e_400;
    if ( ++data >= end ) goto c_33;
l_33: // CO
    if ( *data != 'N' ) goto e_400;
    if ( ++data >= end ) goto c_34;
l_34: // CON
    if ( *data != 'N' ) goto e_400;
    if ( ++data >= end ) goto c_35;
l_35: // CONN
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_36;
l_36: // CONNE
    if ( *data != 'C' ) goto e_400;
    if ( ++data >= end ) goto c_37;
l_37: // CONNEC
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_38;
l_38: // CONNECT
    if ( *data != ' ' ) goto e_400;
    req_type = CONNECT;
    goto burl;


// ----------------- URL -----------------
a_url:
    ++data;
b_url:
    if ( data >= end ) goto c_39;
l_url: // URL, first call
    url_data = data;
    while ( true ) {
        if ( ++data == end ) {
            const char *old = url_data;
            url_size = data - url_data;
            url_rese = 2 * size_buff;
            url_data = (char *)malloc( url_rese );
            memcpy( url_data, old, url_size );
            goto c_40;
        }
        if ( *data == ' ' ) {
            url_size = data - url_data;
            *data = 0;
            goto eurl;
        }
    }


m_url: // URL, cont
    while ( true ) {
        if ( ++data == end )
            goto c_40;
        if ( url_size == url_rese ) {
            char *old = url_data;
            url_data = (char *)malloc( url_rese *= 2 );
            memcpy( url_data, old, url_size );
            free( old );
        }

        if ( *data == ' ' ) {
            url_data[ url_size ] = 0;
            goto eurl;
        }
        url_data[ url_size++ ] = *data;
    }

eurl:
    if ( req_type == GET ) {
        cur_inp = -1;
        return req();
    }

// -----------------  ------------------


// ----------------- ERRORS ------------------

// e_...
#define ERR( NUM, MSG ) e_##NUM: cur_inp = -1; return error_##NUM();
#include "ErrorCodes.h"
#undef ERR

// c_...
#include "HttpRequest_conts.h"

b_100:
    return;
}

#define ERR( NUM, MSG ) \
    void HttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\n\n"; \
        send( s, sizeof( s ) - 1 ); \
    }

#include "ErrorCodes.h"

#undef ERR
