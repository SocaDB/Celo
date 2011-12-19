#include "SocketHelp.h"
#include "EventLoop.h"
#include "Signal.h"

#include <sys/signalfd.h>
#include <signal.h>
#include <stdio.h>

static int make_signal_fd( const int *sigs ) {
    sigset_t mask;
    sigemptyset( &mask );
    for( int i = 0; sigs[ i ] >= 0; ++i )
        sigaddset( &mask, sigs[ i ] );
    if ( sigprocmask( SIG_BLOCK, &mask, 0 ) == -1 ) {
        perror( "sigprocmask" );
        return -1;
    }

    int fd = signalfd( -1, &mask, 0 );
    if ( fd < 0 )
        perror( "signalfd" );
    return fd;
}


SignalAncestor::SignalAncestor( const int *sigs ) : EventObj( make_signal_fd( sigs ) ) {
}

bool SignalAncestor::inp() {
    signalfd_siginfo sig_info;
    while ( true ) {
        ssize_t s = read( fd, &sig_info, sizeof( sig_info ) );
        if ( s < sizeof( sig_info ) ) {
            fprintf( stderr, "Signal error\n" );
            return false;
        }

        if ( signal( sig_info.ssi_signo ) ) {
            ev_loop->stop();
            return true;
        }
    }
    return false;
}
