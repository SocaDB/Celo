#include <Celo/HttpRequest.h>
#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Listener.h>
#include <stdlib.h>

struct MyObserver {
    EventObj *event_obj_factory( int fd ) {
        return new HttpRequest( fd );
    }
};

struct MyListener : Listener<MyObserver> {
    MyListener() : Listener<MyObserver>( &mo, "8888" ) {}

    virtual void rdy() {
        system( "google-chrome http://localhost:8888 &" );
    }

    MyObserver mo;
};

int main() {
    EventLoop el;
    el << new MyListener;
    return el.run();
}

