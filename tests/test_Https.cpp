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


#include <Celo/Events/ConnectionFactoryListener.h>
#include <Celo/Events/SslBufferedConnection.h>
#include <Celo/Util/StringHelp.h>
#include <Celo/Util/SslCtx.h>
#include <Celo/EventLoop.h>

struct MyHttpsRequest : Celo::Events::SslBufferedConnection {
    MyHttpsRequest( SSL_CTX *ssl_ctx, int fd ) : Celo::Events::SslBufferedConnection( ssl_ctx, fd ) {
        PRINT( "new" );
    }

    virtual ~MyHttpsRequest() {
        PRINT( "destroy" );
    }

    virtual void hup() {
        PRINT( "hup" );
    }

    virtual void err() {
        PRINT( "err" );
    }


    virtual bool parse( char *beg, char *end ) {
        std::cout.write( beg, end - beg );
        PRINT( end - beg );

        if ( beg[ 0 ] == 'G' ) {
            this->write_cst(
                    "HTTP/1.0 200 OK\nContent-Type: text/html\n\n"
                    "<html>\n"
                    " <head>\n"
                    "  <title>test</title>\n"
                    "  <meta http-equiv='content-type' content='text/html; charset=UTF-8'/>\n"
                    "  <script type='text/javascript'>\n"
                    "   upload_func = function( data ) {\n"
                    "    var f = document.getElementById( 'yop' ).files[0];\n"
                    "    var x = new XMLHttpRequest();\n"
                    "    x.open( 'PUT', '/', true );\n"
                    "    x.send( f );\n"
                    "   }\n"
                    "  </script>\n"
                    " </head>\n"
                    " <body>\n"
                    "   <input type='file' name='yop' id='yop'/>\n"
                    "   <input type='submit' value='Upload' onclick='upload_func()'/>\n"
                    " </body>\n"
                    "</html>\n"
            ); // enctype='multipart/form-data'  method='post'
            return false;
        }

        if ( beg[ 0 ] == 'P' ) {
            this->write_cst( "HTTP/1.0 200 OK\n\n" );
            // TODO : close the connection !
            // or at least
        }

        return true;
    }
};

int main() {
    using namespace Celo;
    EventLoop el;

    SslCtx ssl_ctx( "mycert.pem", "mycert.pem" );
    Events::ConnectionFactoryListener<MyHttpsRequest,SSL_CTX *> l( "8888", ssl_ctx.ctx );
    el << &l;

    return el.run();
}


