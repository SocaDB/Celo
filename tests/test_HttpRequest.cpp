#include <Celo/HttpRequest.h>
#include <Celo/StringHelp.h>
#include <stdlib.h>
#include <string.h>

void test_HttpRequest( const char *dat_ ) {
    char *data = strdup( dat_ );
    std::cout << dat_ << std::endl;

    HttpRequest req( 0 );
    req.inp( data, data + strlen( data ) );
    PRINT( req.req_type );
    PRINT( req.url_dat );

    HttpRequest rer( 0 );
    for( int i = 0; data[ i ]; ++i ) {
        //PRINT( rer.cur_proc );
        if ( rer.inp( data + i, data + i + 1 ) )
            break;
    }
    PRINT( rer.req_type );
    PRINT( req.url_dat );

    free( data );
}

int main() {
    test_HttpRequest( "GET / HTTP/1.1"  );
    test_HttpRequest( "POST /nouille HTTP/1.1" );
    test_HttpRequest( "PUT /toto? HTTP/1.1"  );
    test_HttpRequest( "DELETE /yop HTTP/1.1"  );
}


