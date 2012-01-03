#include <Celo/HttpRequest_FileServer.h>
#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Listener.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct MyHttpRequest : public HttpRequest_FileServer {
    MyHttpRequest( int fd ) : HttpRequest_FileServer( fd ) {}
    virtual void req() {
        PRINT( url_data );
        if ( strcmp( url_data, "/exit" ) == 0 )
            return ev_loop->stop();
        HttpRequest_FileServer::req();
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

