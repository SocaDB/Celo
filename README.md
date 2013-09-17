What is Celo ?
===========

Celo is an event loop (as for example libevent). It is basically an interface to syscalls like epoll, timerfd, ... to execute callbacks for sockets, timers, signals and so on.

It may be used as a main loop for a server (maybe the main usage), a GUI, etc...

Currently, it basically supports
* raw sockets (you are informed of changes and you decide how to make the interactions),
* periodic (cancelable) timers,
* signals.

Optionnally you can find classes to help manage
* asynchronous buffered IO (with zero-copy file transmission) with optional filtering,
* SSL (via OpenSSL) e.g. for https,
* pools of (restartable and cancelable) one shot timers
* HTTP (fast) parsing (thanks to [SIPE|...]), different kind of HTTP servers, ...

Celo is programmed in C++ with clarity and efficiency in mind (but maybe there is room for improvements).

Celo was created

Some samples
==========

A raw HTTP server

```
struct MyHttpRequest : Celo::Writable {
    MyHttpRequest() : Celo::Writable( fd ) {}
    virtual bool parse( const char *beg, const char *end ) {
        this->write_cst( "HTTP/1.0 200 OK\nContent-Type: text/plain\n\nHello" );
        return false;
    }
};

int main() {
    Celo::EventLoop el;

    Listener_Factory<MyHttpRequest> l( "8888" );
    el << &l; // the loop will look for new connections

    return el.run();
}
```
