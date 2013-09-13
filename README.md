What is Celo ?
===========

Celo is an event loop (as e.g. libevent, libev, ...). It is an interface to syscall like epoll (if linux) to execute callbacks for new data on (or availability of) sockets, timeouts, signals (SIGxxx), ...

It may be used as a main loop for a server (maybe the main usage), a GUI, ...

Currently, it supports
* raw sockets (you are informed of changes and you decide how to read, write, ...),
* periodic (cancelable) timers,
* signals.

Optionnally you can find classes to help for
* asynchronous reads and writes (with optional filtering),
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
