#include <Celo/System/Buffer.h>

using namespace Celo;

int main() {
    Ptr<Buffer> b = new Buffer;
    b->data[ 0 ] = 10;
    b->used = 1;

    PRINT( b );
}
