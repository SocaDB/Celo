#ifndef CELO_FILTER_H
#define CELO_FILTER_H

#include "../TypeConfig.h"

namespace Celo {

/**
*/
class Filter {
public:
    struct OutputFunctor {
        virtual bool operator()( const char *data, ST size, bool end ) = 0; ///< new data. return true if OK to continue
    };

    virtual ~Filter() {}
    virtual bool operator()( OutputFunctor &out_func, const char *data, ST size, bool end = false ) = 0; ///< new data chunck. return true if OK to continue
    virtual bool update( OutputFunctor &out_func ) {} ///< called after all data chuncks has been sent. return true if OK to continue
};

}

#endif // CELO_FILTER_H
