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
#include "../EventLoop.h"
#include "Signal.h"

#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

namespace Celo {
namespace Events {

static int make_signal_fd( const int *sigs ) {
    // set up a mask
    sigset_t mask;
    sigemptyset( &mask );
    for( int i = 0; sigs[ i ] >= 0; ++i )
        sigaddset( &mask, sigs[ i ] );

    // block signals
    if ( sigprocmask( SIG_BLOCK, &mask, 0 ) == -1 ) {
        perror( "sigprocmask" );
        return -1;
    }

    // get a (non blocking) file descriptor
    int fd = signalfd( -1, &mask, 0 );
    if ( fd < 0 )
        perror( "signalfd" );
    if ( set_non_blocking( fd ) < 0 ) {
        perror( "non blocking signalfd" );
        close( fd );
        return -1;
    }

    return fd;
}


Signal::Signal( const int *sigs ) : Event( make_signal_fd( sigs ) ) {
}

void Signal::inp() {
    signalfd_siginfo sig_info;
    while ( true ) {
        ssize_t s = read( fd, &sig_info, sizeof( sig_info ) );

        // end of the connection
        if ( s == 0 )
            return;

        // error
        if ( s < 0 ) {
            if ( errno == EAGAIN or errno == EWOULDBLOCK )
                return wait_for_more_inp();
            return perror( "Pb reading signals" );
        }

        if ( s < (ssize_t)sizeof( sig_info ) ) {
            fprintf( stderr, "TODO: partial read with signals\n" );
            return;
        }

        if ( not signal( sig_info.ssi_signo ) )
            return;
    }
}

} // namespace Events
} // namespace Celo
