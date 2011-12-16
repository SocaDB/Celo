#include "EventObj.h"

#include <sys/socket.h>
#include <unistd.h>

EventObj::EventObj( int fd ) : fd( fd ) {
    rem_out = 0;
}

EventObj::~EventObj() {
    if ( fd >= 0 )
        close( fd );
}

bool EventObj::inp() {
    return false;
}

bool EventObj::out() {
    //    if ( rem_size_out ) {
    //        int real = ::send( fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
    //        if ( real < 0 )
    //            continue;
    //        size -= real;
    //        data += real;

    //    }
    return false;
}

void EventObj::err() {
}

void EventObj::hup() {
}

bool EventObj::send( const char *data, ST size, bool end ) {
    return false;
}

bool EventObj::send( const std::string &str, bool end ) {
    send( str.c_str(), str.size(), end );
}
