#ifndef STRINGCPN_H
#define STRINGCPN_H

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

    void own_copy( int suggested_rese ) {
        const char *old = data;
        rese = suggested_rese;
        data = (char *)malloc( rese );
        memcpy( data, old, size );
    }

    char *data;
    int   rese;
    int   size;
};


#endif // STRINGCPN_H
