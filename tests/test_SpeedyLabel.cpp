#include <Celo/StringHelp.h>

int main( int argc, char **argv ) {
    const char *data = *argv;
//    if ( *data == 'E' ) return 1;
//    if ( *data == 'A' ) return 10;
//    if ( *data == 'B' ) return 100;
    switch ( *data ) {
    case 'E': return 1;
    case 'A': return 10;
    case 'B': return 100;
    }
    return 0;
}

