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


#include <Celo/EventObj_WO_SSL.h>
#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Listener.h>
#include <Celo/SslCtx.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct MyHttpsRequest : EventObj_WO_SSL {
    MyHttpsRequest( SSL_CTX *ssl_ctx, int fd ) : EventObj_WO_SSL( ssl_ctx, fd ) {
    }

    virtual int parse( char *beg, char *end ) {
        write( 0, beg, end - beg );
        return OK;
    }
};

struct MyListener : Listener {
    MyListener( const char *port, SslCtx *ssl_ctx ) : Listener( port ), ssl_ctx( ssl_ctx ) {
    }

    virtual void rdy() {
        system( "google-chrome https://localhost:8899 &" );
    }

    virtual EventObj *event_obj_factory( int fd ) {
        return new MyHttpsRequest( ssl_ctx->ctx, fd );
    }

    SslCtx *ssl_ctx;
};

int main() {
    EventLoop el;

    SslCtx ssl_ctx( "mycert.pem", "mycert.pem" );
    MyListener l( "8899", &ssl_ctx );
    el << &l;

    return el.run();
}


