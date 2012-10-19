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


#include <Celo/BasicHttpRequest_FileServer.h>
#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Listener.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct MyHttpRequest : EventObj_WO {
    MyHttpRequest( int fd ) : EventObj_WO( fd ) {
    }

    virtual bool inp( char *beg, char *end ) {
        write( 0, beg, end - beg );
        std::cout.write( beg, end - beg );
        return false;
        //        if ( strcmp( url.data, "/exit" ) == 0 )
        //            return ev_loop->stop();
        //        BasicHttpRequest_FileServer::req_GET();
    }
};

struct MyListener : Listener {
    MyListener( const char *port ) : Listener( port ) {}

    virtual void rdy() {
        system( "google-chrome http://localhost:8899 &" );
    }

    virtual EventObj *event_obj_factory( int fd ) {
        return new MyHttpRequest( fd );
    }
};

int main() {
    EventLoop el;

    MyListener l( "8899" );
    el << &l;

    return el.run();
}

