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


#include <Celo/Util/StringHelp.h>
#include <Celo/EventLoop.h>
#include <signal.h>
#include <fcntl.h>

#include <Celo/Listener_Factory.h>
#include <Celo/Listener_WO.h>
#include <Celo/Parsable_WO.h>
#include <Celo/Signal_WO.h>
#include <Celo/Timer_WO.h>

struct MyObserver {
    bool timeout( Celo::Timer *eo, int nb_expirations ) {
        PRINT( nb_expirations );
        return true;
    }

    bool signal( Celo::Signal *eo, int sig ) {
        PRINT( sig );
        if ( sig == SIGINT )
            eo->ev_loop->stop( 10 );
        return true;
    }

    template<class EO>
    bool connection( EO *eo, int fd ) {
        *eo->ev_loop << new Celo::Parsable_WO<MyObserver,int>( this, fd, eo->data );
        return true;
    }

    template<class EO>
    bool parse( EO *eo, char *beg, char *end ) {
        std::cout << "Incoming Data: (data attribute=" << eo->data << ")\n";
        std::cout;
        std::cout.write( beg, end - beg );

        eo->write_cst( "HTTP/1.0 200 OK\nContent-Type: text/plain\n\nHello" );
        return false;
    }
};


int main() {
    using namespace Celo;
    EventLoop el;
    MyObserver mo;

    int sigs[] = { SIGINT, SIGQUIT, SIGKILL, SIGUSR1, -1 };
    el << new Signal_WO<MyObserver>( &mo, sigs );
    
    el << new Timer_WO<MyObserver>( &mo, 1 );

    el << new Listener_WO<MyObserver,int>( &mo, "8888", 16 );

    // for a test (not a really convenient example)
    el << new Listener_Factory<Parsable_WO<MyObserver,int>,MyObserver *>( "8889", &mo );

    return el.run();
}
