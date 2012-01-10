#include <Celo/BasicHttpRequest_FileServer.h>
#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Listener.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct MyHttpRequest : public BasicHttpRequest_FileServer {
    MyHttpRequest( int fd ) : BasicHttpRequest_FileServer( fd ) {
    }
    virtual void req_GET() {
        PRINT( url.data );
        if ( strcmp( url.data, "/exit" ) == 0 )
            return ev_loop->stop();
        BasicHttpRequest_FileServer::req_GET();
    }
};

struct MyObserver {
    EventObj *event_obj_factory( int fd ) {
        return new MyHttpRequest( fd );
    }
};

struct MyListener : Listener<MyObserver> {
    MyListener() : Listener<MyObserver>( &mo, "8899" ) {}

    virtual void rdy() {
        system( "google-chrome http://localhost:8899 &" );
    }

    MyObserver mo;
};

int main() {
    if ( int res = chdir( "html" ) ) {
        perror( "chdir" );
        return res;
    }

    MyListener l;
    EventLoop el;
    el << &l;
    return el.run();
}

