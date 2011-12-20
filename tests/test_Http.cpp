#include <Celo/HttpRequest.h>
#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Listener.h>

struct MyObserver {
    bool timeout() {
        std::cout << "timer" << std::endl;
        return false;
    }
    bool signal( int sig ) {
        std::cout << "sig " << sig << std::endl;
        return true;
    }
    EventObj *event_obj_factory( int fd ) {
        return new HttpRequest( fd );
    }
};


int main() {
    EventLoop el;

    MyObserver mo;
    el << new Listener<MyObserver>( &mo, "8888" );

    return el.run();
}

