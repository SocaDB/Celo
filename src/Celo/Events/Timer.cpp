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


#include "../Util/FileDescriptor.h"
#include "Timer.h"

#include <sys/timerfd.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name rt
#endif // METIL_COMP_DIRECTIVE

namespace Celo {
namespace Events {

static int make_timer_fd( double delay ) {
    timespec now;
    if ( clock_gettime( CLOCK_REALTIME, &now ) == -1 ) {
        perror( "clock_gettime" );
        return -1;
    }

    //
    int fd = timerfd_create( CLOCK_REALTIME, 0 );
    if ( fd == -1 ) {
        perror( "timerfd_create" );
        return fd;
    }

    if ( set_non_blocking( fd ) == -1 ) {
        perror( "non blocking timer fd" );
        close( fd );
        return -1;
    }

    itimerspec new_value;
    new_value.it_value.tv_sec = now.tv_sec + 1;
    new_value.it_value.tv_nsec = now.tv_nsec;
    new_value.it_interval.tv_sec = floor( delay );
    new_value.it_interval.tv_nsec = 1e9 * ( delay - floor( delay ) );
    if ( timerfd_settime( fd, TFD_TIMER_ABSTIME, &new_value, NULL ) == -1 ) {
        perror( "timerfd_settime" );
        close( fd );
        return -1;
    }

    return fd;
}


Timer::Timer( double delay ) : Event( make_timer_fd( delay ) ) {
}

void Timer::inp() {
    uint64_t nb_expirations;
    ssize_t s = read( fd, &nb_expirations, sizeof( uint64_t ) );
    if ( timeout( s >= sizeof( uint64_t ) ? nb_expirations : 1 ) )
        wait_for_more_inp();
}

} // namespace Events
} // namespace Celo
