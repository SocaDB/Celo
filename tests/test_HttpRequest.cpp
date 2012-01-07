#include <Celo/HttpRequest.h>
#include <Celo/StringHelp.h>
#include <stdlib.h>
#include <string.h>

void test_HttpRequest( const char *cata, int chunk_size ) {
    char *data = strdup( cata );

    HttpRequest rer( 0 );
    for( int i = 0; i < strlen( data ); i += chunk_size ) {
        rer.inp( data + i, data + std::min( i + chunk_size, (int)strlen( data ) ) );
        if ( rer.end() )
            break;
    }
    std::cout << " -> " << " u=" << rer.url.data << " l=" << rer.content_length << std::endl;

    free( data );
}

void test_HttpRequest( const char *data ) {
    std::cout << data << std::endl;
    test_HttpRequest( data, 1024 );
    test_HttpRequest( data, 1 );
}

int main() {
    test_HttpRequest( "GET / HTTP/1.1\npouet\n\n"  );
    test_HttpRequest( "POST /nouille HTTP/1.1\nContent-Length: 3495\n\n" );
    //test_HttpRequest( "POST /nouille HTTP/1.1\r\nContent-Length: 3495\r\n\r\n" );
    //test_HttpRequest( "PUT /toto? HTTP/1.1"  );
    //test_HttpRequest( "DELETE /yop HTTP/1.1"  );
}


