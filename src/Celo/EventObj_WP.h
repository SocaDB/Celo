#ifndef EVENTOBJ_WP_H
#define EVENTOBJ_WP_H

#include "EventObj.h"

/**
  EventObj With parse
*/
class EventObj_WP : public EventObj {
public:
    EventObj_WP( int fd );
    virtual bool inp();

protected:
    // to be redefined (if default inp is used)
    virtual int parse( char *beg, char *end ) = 0; ///< return 0 if we need more to read.
};

#endif // EVENTOBJ_WP_H
