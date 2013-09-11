#include "Filtered.h"
#include <unistd.h>
#include <errno.h>

namespace Celo {

Filtered::Filtered( int fd ) : Filtered( fd ) {
}

Filtered::Filtered( VtableOnly vo ) : Filtered( vo ) {
}

bool Filtered::inp() {
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff <= 0 ) {
            // need a retry ?
            if ( ruff < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                return true;
            err();
            return false;
        }

        // parse
        if ( not parse( buff, buff + ruff ) )
            return false;
    }
}

}

