/*
 Copyright 2012 Structure Computation  www.structure-computation.com
 Copyright 2012 Hugo Leclerc

 This file is part of Celo.

 Celo is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Celo is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with Celo. If not, see <http://www.gnu.org/licenses/>.
*/


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
