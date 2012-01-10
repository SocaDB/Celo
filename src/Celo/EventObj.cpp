#include "StringHelp.h"
#include "EventLoop.h"
#include "EventObj.h"

#include <sys/socket.h>
#include <unistd.h>

EventObj::EventObj( int fd ) : fd( fd ) {
}

EventObj::~EventObj() {
    if ( fd > 2 )
        close( fd );
}

bool EventObj::inp() {
    return false;
}

bool EventObj::out() {
    return false;
}

void EventObj::err() {
}

void EventObj::hup() {
}

void EventObj::rdy() {
}

void EventObj::poll_out() {
    ev_loop->poll_out( this );
}
