#include "Listener_SSL.h"

Listener_SSL::Listener_SSL( const char *port ) : EventObj( listening_socket( port ) ) {
}

Listener_SSL::Listener_SSL( VtableOnly vo ) : EventObj( vo ) {
}

bool Listener_SSL::inp() {
    while ( true ) {
        int nd = accepting_socket( fd );
        if ( nd == -1 )
            return true; // continue to listen
        *ev_loop << event_obj_factory( nd );
    }
}

EventObj *event_obj_factory( int fd );
