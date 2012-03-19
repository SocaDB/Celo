#include "EventObj_WP.h"
#include <unistd.h>
#include <errno.h>

EventObj_WP::EventObj_WP( int fd ) : EventObj( fd ) {
}

bool EventObj_WP::inp() {
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff < 0 ) {
            if ( errno == EAGAIN )
                continue;
            return false;
        }

        // we need more data but we don't have ?
        if ( ruff == 0 )
            return true;

        // parse
        // write( 0, buff, ruff );
        if ( parse( buff, buff + ruff ) )
            return false;
    }
}

