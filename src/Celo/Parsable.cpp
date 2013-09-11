#include "Parsable.h"
#include <unistd.h>
#include <errno.h>

namespace Celo {

Parsable::Parsable( int fd ) : EventObj( fd ) {
}

Parsable::Parsable( VtableOnly vo ) : EventObj( vo ) {
}

bool Parsable::inp() {
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff <= 0 ) {
            // closed ?
            if ( ruff == 0 )
                return false;
            // need a retry ?
            if ( errno == EAGAIN or errno == EWOULDBLOCK )
                return true;
            return false;
        }

        // parse
        if ( not parse( buff, buff + ruff ) )
            return false;
    }
}

}
