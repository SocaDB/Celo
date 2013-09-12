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


//#include <Celo/EventObj_WO_SSL.h>
#include <Celo/Listener_Factory.h>
#include <Celo/Util/StringHelp.h>
#include <Celo/Filter_SSL.h>
#include <Celo/EventLoop.h>
#include <Celo/Filtered.h>
#include <Celo/SslCtx.h>
//#include <string.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <stdio.h>

struct MyHttpsRequest : Celo::Filtered {
    MyHttpsRequest( SSL_CTX *ssl_ctx, int fd ) : Celo::Filtered( fd, &f ), f( ssl_ctx ) {
    }

    virtual bool parse( char *beg, char *end ) {
        std::cout.write( beg, end - beg );

        write_cst( "HTTP/1.0 200 OK\nContent-Type: text/plain\n\nHello https" );
        return false;
    }

    Celo::Filter_SSL f;
};

int main() {
    using namespace Celo;
    EventLoop el;

    SslCtx ssl_ctx( "mycert.pem", "mycert.pem" );
    Listener_Factory<MyHttpsRequest,SSL_CTX *> l( "8888", ssl_ctx.ctx );
    el << &l;

    return el.run();
}


