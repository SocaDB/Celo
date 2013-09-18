#ifndef Celo_Filters_IdentityFilter_H
#define Celo_Filters_IdentityFilter_H

#include "Filter.h"

namespace Celo {
namespace Filters {

/**
*/
class Identity : public Filter {
public:
    virtual bool operator()( OutputFunctor &out_func, const char *data, ST size, bool end = false );
};

} // namespace Filters
} // namespace Celo

#endif // Celo_Filters_IdentityFilter_H
