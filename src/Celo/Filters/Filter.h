#ifndef Celo_Filters_Filter_H
#define Celo_Filters_Filter_H

#include "../Sys/TypeConfig.h"

namespace Celo {
namespace Filters {

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

} // namespace Filters
} // namespace Celo

#endif // Celo_Filters_Filter_H
