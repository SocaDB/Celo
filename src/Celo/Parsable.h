#ifndef PARSABLE_H
#define PARSABLE_H

#include "Writable.h"

namespace Celo {

/**
  Event_obj which calls parse( beg, end ) each time new data comes in
*/
class Parsable : public Writable {
public:
    Parsable( VtableOnly vo );
    Parsable( int fd );

protected:
    virtual bool inp();

    // to be redefined
    virtual bool parse( char *beg, char *end ) = 0; ///< return false if we have enough data
};

}

#endif // PARSABLE_H
