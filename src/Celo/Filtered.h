#ifndef CELO_FILTERED_H
#define CELO_FILTERED_H

#include "Parsable.h"
#include "Filter.h"

namespace Celo {

/**
  Event_obj which calls parse( beg, end ) each time new data comes in
*/
class Filtered : public Parsable {
public:
    Filtered( VtableOnly vo );
    Filtered( int fd );

protected:
    virtual bool inp();
};

}

#endif // CELO_FILTERED_H
