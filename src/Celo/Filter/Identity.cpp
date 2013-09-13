#include "Identity.h"

namespace Celo {

bool Identity::operator()( OutputFunctor &out_func, const char *data, ST size, bool end ) {
    return out_func( data, size, end );
}

}
