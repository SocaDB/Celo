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


#include <Celo/Events/BufferedConnection.h>
#include <Celo/Events/Listener.h>
#include <Celo/Util/StringHelp.h>
#include <Celo/EventLoop.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct MyHttpRequest : Celo::Events::BufferedConnection {
    MyHttpRequest( int fd ) : Celo::Events::BufferedConnection( fd ) {
    }

    virtual bool parse( char *beg, char *end ) {
        write( 0, beg, end - beg );
        std::cout.write( beg, end - beg );

        this->write_cst( "HTTP/1.0 200 OK\nContent-Type: text/plain\n\nTest" );

        return false;
    }
};

struct MyListener : Celo::Events::Listener {
    MyListener( const char *port ) : Celo::Events::Listener( port ) {}

    //virtual void rdy() {
    //    system( "google-chrome http://localhost:8899 &" );
    //}

    virtual bool connection( int fd ) {
        *ev_loop << new MyHttpRequest( fd );
        return true;
    }
};

int main() {
    Celo::EventLoop el;

    MyListener l( "8899" );
    el << &l;

    return el.run();
}

