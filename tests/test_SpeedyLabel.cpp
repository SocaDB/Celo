#include <Celo/StringHelp.h>

void g() {}

int f( const char *data, const char *end ) {
l_0:
    if ( *data != 'E' ) goto e_400;
    if ( ++data >= end ) goto c_GE;
l_1:
    if ( *data != 'T' ) goto e_400;
    if ( ++data >= end ) goto c_GET;
l_2:
    if ( *data != 'O' ) goto e_400;
    if ( ++data >= end ) goto c_GETO;

e_400:
    g();
c_GE:
    g();
c_GET:
    g();
c_GETO:
    g();
}

int main( int argc, char **argv ) {
    return f( argv[ 0 ], argv[ 1 ] );
}

