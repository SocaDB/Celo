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


#include "../EventLoop.h"
#include "Event.h"

#include <unistd.h>

namespace Celo {
namespace Events {

Event::Event( int fd ) : fd( fd ), next_ev_to_del( 0 ), waiting( 0 ), errors( 0 ) {
}

Event::Event( VtableOnly vo ) {
}

Event::~Event() {
    if ( fd > 2 )
        close( fd );
}

void Event::inp() {
}

void Event::out() {
}

void Event::rdy() {
}

void Event::poll_out() {
    ev_loop->poll_out_obj( this );
}

void Event::reg_for_deletion() {
    if ( next_ev_to_del )
        return;
    next_ev_to_del = ev_loop->last_ev_to_del ? ev_loop->last_ev_to_del : this;
    ev_loop->last_ev_to_del = this;
}

} // namespace Events
} // namespace Celo
