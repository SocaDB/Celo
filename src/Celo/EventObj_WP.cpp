#include "EventObj_WP.h"
#include <unistd.h>
#include <errno.h>

#include "StringHelp.h"

EventObj_WP::EventObj_WP( int fd ) : EventObj( fd ) {
}

EventObj_WP::EventObj_WP( VtableOnly vo ) : EventObj( vo ) {
}

bool EventObj_WP::inp() {
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff < 0 ) {
            // EAGAIN
            if ( errno == EAGAIN )
                return true;
            return false;
        }

        // we need more data but we don't have ?
        if ( ruff == 0 )
            return true;

        // parse
        // write( 0, buff, ruff );
        int p = parse( buff, buff + ruff );
        PRINT( p );
        
        if ( p )
            return p < 0;
    }
}

bool EventObj_WP::cnt_default_value() const {
    return false;
}


