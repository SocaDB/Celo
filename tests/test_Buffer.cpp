#include <Celo/System/Buffer.h>
#include <iostream>

using namespace Celo;

int main() {
    Ptr<Buffer> b = new Buffer;
    b->data[ 0 ] = 10;
    b->data[ 1 ] = 10;
    b->used = 2;


    b->write_to_stream( std::cout );
    std::cout << std::endl;
}
