#include "StringHelp.h"

std::string url_encode( const std::string &str ) {
    std::string res;
    res.reserve( str.size() );
    for( int i = 0; i < str.size(); ++i ) {
        if ( str[ i ] == ' ' )
            res += "%20";
        else
            res += str[ i ];
    }
    return res;
}
