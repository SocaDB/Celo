#include "HttpRequest.h"
#include "StringHelp.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const int size_buff = 1024;

HttpRequest::HttpRequest( int fd ) : EventObj( fd ), cur_proc( 0 ), url_res( 0 ) {
}

HttpRequest::~HttpRequest() {
    if ( url_res )
        free( url_dat );
}

bool HttpRequest::inp() {
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );        
        if ( ruff < 0 ) {
            if ( errno == EAGAIN )
                continue;
            return true;
        }
        if ( ruff == 0 )
            return false;

        // parse
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


bool HttpRequest::inp( char *data, const char *end ) {
    // switch ...
    #include "HttpRequest_jumps.h"


// ----------------- REQ -----------------
l_0: //
    // we have at least 4 bytes ?
    if ( end - data >= 4 ) {
        int val = *reinterpret_cast<const int *>( data );

        #ifdef __LITTLE_ENDIAN
        if ( val == 0x20544547 ) { data += 4; req_type = GET; goto bur_; } // "GET "
        if ( val == 0x54534F50 ) { data += 4; goto b_POST; } // "POST"
        if ( val == 0x20545550 ) { data += 4; req_type = PUT; goto bur_; } // "PUT "
        #else
        if ( val == 0x47455420 ) { data += 4; req_type = GET; goto bur_; } // "GET "
        if ( val == 0x504F5354 ) { data += 4; goto b_POST; } // "POST"
        if ( val == 0x50555420 ) { data += 4; req_type = PUT; goto bur_; } // "PUT "
        #endif
    }

    if ( *data == 'P' ) goto b_4;
    if ( *data == 'D' ) goto b_10;
    if ( *data == 'O' ) goto b_16;
    if ( *data == 'H' ) goto b_23;
    if ( *data == 'T' ) goto b_27;
    if ( *data == 'C' ) goto b_32;
    if ( *data != 'G' ) goto e_400;
    if ( ++data >= end ) goto c_1;
l_1: // G
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_2;
l_2: // GE
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_3;
l_3: // GET
    if ( *data != ' ' ) goto e_400;
    req_type = GET;

// ----------------- URL -----------------
burl:
    ++data;
bur_:
    if ( data >= end ) goto c_39;
l_39: // URL, first call
    url_dat = data;
    while ( true ) {
        if ( ++data == end ) {
            const char *old = url_dat;
            url_len = data - url_dat;
            url_res = 2 * size_buff;
            url_dat = (char *)malloc( url_res );
            memcpy( url_dat, old, url_len );
            goto c_40;
        }
        if ( *data == ' ' ) {
            url_len = data - url_dat;
            *data = 0;
            goto b_100;
        }
    }


l_40: // URL, cont
    while ( true ) {
        if ( ++data == end )
            goto c_40;
        if ( url_len == url_res ) {
            char *old = url_dat;
            url_dat = (char *)malloc( url_res *= 2 );
            memcpy( url_dat, old, url_len );
            free( old );
        }

        if ( *data == ' ' ) {
            url_dat[ url_len ] = 0;
            goto b_100;
        }
        url_dat[ url_len++ ] = *data;
    }

// -------------- REQ cont ---------------
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

// e_...
#define ERR( NUM, MSG ) e_##NUM: error_##NUM(); return true;
#include "ErrorCodes.h"
#undef ERR

// c_...
#include "HttpRequest_conts.h"

b_100:
    return 0;
}

#define ERR( NUM, MSG ) \
    void HttpRequest::error_##NUM() { \
        const char s[] = "HTTP/1.0 " #NUM " " MSG "\n\n"; \
        send( s, sizeof( s ) - 1 ); \
    }

#include "ErrorCodes.h"

#undef ERR
