#include "UrlEncodeAndDecode.h"

namespace Celo {

/*
import string
s = ""
for i in range( 256 ):
    c = chr( i )
    if c.isalnum() or c == '-' or c == '_' or c == '.' or c == '~':
        s += c
    else:
        s += "%"

print 'static char conv[] = "' + s + '";'

# from hex
import string
s = []
for i in range( 256 ):
    c = chr( i )
    if c >= '0' and c <= '9':
        s.append( str( i - ord( '0' ) ) )
    elif c >= 'a' and c <= 'f':
        s.append( str( 10 + i - ord( 'a' ) ) )
    elif c >= 'A' and c <= 'F':
        s.append( str( 10 + i - ord( 'A' ) ) )
    else:
        s.append( '0' )

print 'static char from_hex[] = { ' + string.join( s, ',' ) + ' };'

*/

std::string url_encode( const std::string &str ) {
    static char conv[] = "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%-.%0123456789%%%%%%%ABCDEFGHIJKLMNOPQRSTUVWXYZ%%%%_%abcdefghijklmnopqrstuvwxyz%%%~%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
    static char to_hex[] = "0123456789ABCDEF";
    std::string res; res.reserve( str.size() );
    for( unsigned i = 0; i < str.size(); ++i ) {
        unsigned char u = str[ i ];
        char c = conv[ u ];
        if ( c == '%' ) {
            res += '%'; res += to_hex[ u >> 4 ]; res += to_hex[ u & 15 ];
        } else
            res += c;
    }
    return res;
}

std::string url_decode( const std::string &str ) {
    static char from_hex[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::string res;
    for( unsigned i = 0; i < str.size(); ++i ) {
        if ( str[ i ] == '%' and i + 2 < str.size() ) {
            res += ( from_hex[ (unsigned char)str[ i + 1 ] ] << 4 ) | from_hex[ (unsigned char)str[ i + 2 ] ];
            i += 2;
        } else
            res += str[ i ];
    }

    return res;
}

}
