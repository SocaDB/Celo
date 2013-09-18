#include "IdentityFilter.h"

namespace Celo {
namespace Filters {

bool Identity::operator()( OutputFunctor &out_func, const char *data, ST size, bool end ) {
    return out_func( data, size, end );
}

} // namespace Filters
} // namespace Celo
