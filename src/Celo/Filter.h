#ifndef CELO_FILTER_H
#define CELO_FILTER_H

#include "TypeConfig.h"

namespace Celo {

/**
*/
class Filter {
public:
    enum { NEED_MORE_INPUT = -1, END_OF_THE_CONNECTION = -3, ERROR = -4 };

    virtual ~Filter() {}

    virtual ST new_inp( const char *data, ST size ) = 0; ///< send input data to filter (e.g. what comes from a socket)
    virtual ST new_out( const char *data, ST size ) = 0; ///< send data to output

    virtual ST get_inp( char *data, ST max_size ) = 0;   ///< input data that have been filtered. May return NEED_MORE_xx, ....
    virtual ST get_out( char *data, ST max_size ) = 0;   ///< output data that have been filtered. May return NEED_MORE_xx, ....
};

}

#endif // CELO_FILTER_H
