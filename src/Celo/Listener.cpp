#include "StringHelp.h"
#include "SocketHelp.h"
#include "EventLoop.h"
#include "Listener.h"

Listener::Listener( const char *port ) : EventObj( listening_socket( port ) ) {
}

Listener::Listener( int fd ) : EventObj( fd ) {
}

bool Listener::inp() {
    while ( true ) {
        int nd = accepting_socket( fd );
        if ( nd == -1 )
            return true; // continue to listen
        *ev_loop << event_obj_factory( nd );
    }
}
