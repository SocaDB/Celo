#ifndef STRINGCPN_H
#define STRINGCPN_H

#include <algorithm>
#include <stdlib.h>
#include <string.h>

/**
*/
struct StringCpn {
    StringCpn() : rese( 0 ) {
    }

    ~StringCpn() {
        if ( rese )
            free( data );
    }

    /// make an own copy if it's not the case (rese=0 means that data is not owned by this)
    void make_own_copy_if_not_the_case() {
        if ( rese )
            return;
        const char *old = data;
        rese = std::max( 1024, 2 * size );
        data = (char *)malloc( rese );
        memcpy( data, old, size );
    }

    /// ensure room for at least 1 char
    void make_room_for_at_least_1_char() {
        if ( size == rese ) {
            char *old = data;
            data = (char *)malloc( rese *= 2 );
            memcpy( data, old, size );
            free( old );
        }
    }


    char *data;
    int   rese;
    int   size;
};


#endif // STRINGCPN_H
