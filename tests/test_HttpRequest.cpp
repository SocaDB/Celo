#include <Celo/BasicHttpRequest.h>
#include <Celo/StringHelp.h>
#include <stdlib.h>
#include <string.h>

struct TestHttpRequest : BasicHttpRequest {
    TestHttpRequest( const char *str, int chunk_size ) : BasicHttpRequest( 0 ) {
        char *data = strdup( str );

        for( int i = 0; i < strlen( data ); i += chunk_size )
            if ( not inp( data + i, data + std::min( i + chunk_size, (int)strlen( data ) ) ) )
                break;
        std::cout << " -> " << " u=" << url.data << " l=" << content_length << std::endl;

        free( data );
    }
};

void test_BasicHttpRequest( const char *cata, int chunk_size ) {
}

void test_BasicHttpRequest( const char *data ) {
    std::cout << data << std::endl;
    TestHttpRequest( data, 1024 );
    TestHttpRequest( data, 1 );
}

int main() {
    //test_BasicHttpRequest( "GET /rotiapum HTTP/1.1\npouet\n\n"  );
    test_BasicHttpRequest( "POST /nouille HTTP/1.1\nContent-Length: 3495\n\ncouillation" );
    //test_BasicHttpRequest( "POST /nouille HTTP/1.1\r\nContent-Length: 3495\r\n\r\n" );
    //test_BasicHttpRequest( "PUT /toto? HTTP/1.1"  );
    //test_BasicHttpRequest( "DELETE /yop HTTP/1.1"  );
}


