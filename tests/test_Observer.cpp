#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Signal_WO.h>
#include <Celo/Timer_WO.h>
#include <signal.h>
#include <iostream>

struct MyObserver {
    bool timeout() {
        std::cout << "pouet" << std::endl;
        return false;
    }
    bool signal( int sig ) {
        std::cout << "sig " << sig << std::endl;
        return true;
    }
};


int main() {
    EventLoop el;
    MyObserver mo;

    int sigs[] = { SIGINT, SIGQUIT, SIGKILL, -1 };
    el << new Signal_WO<MyObserver>( &mo, sigs );
    
    el << new Timer_WO<MyObserver>( &mo, 1 );

    return el.run();
}
