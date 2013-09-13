#ifndef IDENTITY_H
#define IDENTITY_H

#include "Filter.h"

namespace Celo {

/**
*/
class Identity : public Filter {
public:
    virtual bool operator()( OutputFunctor &out_func, const char *data, ST size, bool end = false );
};

}

#endif // IDENTITY_H
