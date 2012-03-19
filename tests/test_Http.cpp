#include <Celo/BasicHttpRequest_FileServer.h>
#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Listener.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct MyHttpRequest : EventObj_WO {
    MyHttpRequest( int fd ) : EventObj_WO( fd ) {
    }

    virtual bool inp( char *beg, char *end ) {
        write( 0, beg, end - beg );
        std::cout.write( beg, end - beg );
        return false;
        //        if ( strcmp( url.data, "/exit" ) == 0 )
        //            return ev_loop->stop();
        //        BasicHttpRequest_FileServer::req_GET();
    }
};

struct MyListener : Listener {
    MyListener( const char *port ) : Listener( port ) {}

    virtual void rdy() {
        system( "google-chrome http://localhost:8899 &" );
    }

    virtual EventObj *event_obj_factory( int fd ) {
        return new MyHttpRequest( fd );
    }
};

int main() {
    EventLoop el;

    MyListener l( "8899" );
    el << &l;

    return el.run();
}

