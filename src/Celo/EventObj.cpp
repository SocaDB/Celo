#include "EventObj.h"

#include <sys/socket.h>
#include <unistd.h>

EventObj::EventObj( int fd ) : fd( fd ) {
}

EventObj::~EventObj() {
    if ( fd > 0 )
        close( fd );
}

void EventObj::inp() {
    if ( end() )
        delete this;
}

void EventObj::out() {
    if ( end() )
        delete this;
}

void EventObj::err() {
    delete this;
}

void EventObj::hup() {
    delete this;
}

void EventObj::rdy() {
}

bool EventObj::end() {
    return true;
}

