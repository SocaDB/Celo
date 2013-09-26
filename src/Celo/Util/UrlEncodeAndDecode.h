#ifndef URLENCODEANDDECODE_H
#define URLENCODEANDDECODE_H

#include <string>

namespace Celo {

inline bool is_a_number( char a ) { return a >= '0' and a <= '9'; }
std::string url_encode( const std::string &str );
std::string url_decode( const std::string &str );

}

#endif // URLENCODEANDDECODE_H
