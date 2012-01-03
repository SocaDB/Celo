#include "SocketHelp.h"
#include "Timer.h"

#include <sys/timerfd.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#pragma lib_name rt

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


TimerAncestor::TimerAncestor( double delay ) : EventObj( make_timer_fd( delay ) ) {
}

void TimerAncestor::inp() {
    uint64_t exp;
    ssize_t s = read( fd, &exp, sizeof( uint64_t ) );
    if ( s != sizeof( uint64_t ) )
        fprintf( stderr, "timer error\n" );

    timeout();
}

bool TimerAncestor::end() {
    return false;
}
