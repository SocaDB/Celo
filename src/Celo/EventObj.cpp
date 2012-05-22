#include "StringHelp.h"
#include "EventLoop.h"
#include "EventObj.h"

#include <sys/socket.h>
#include <unistd.h>

EventObj::EventObj( int fd ) : fd( fd ) {
}

EventObj::EventObj( VtableOnly vo ) {
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


bool EventObj::want_poll_inp_at_the_beginning() const {
    return true;
}

bool EventObj::want_poll_out_at_the_beginning() const {
    return false;
}

bool EventObj::cnt_default_value() const {
    return true;
}

bool EventObj::called_if_idle() const {
    return false;
}
