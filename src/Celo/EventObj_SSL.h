#ifndef EVENTOBJ_SSL_H
#define EVENTOBJ_SSL_H

#include "EventObj.h"

/**
*/
class EventObj_SSL : public EventObj {
public:
    enum { WAIT = -1, CNT = 0, OK = 1, KO = 2 };
    EventObj_SSL( VtableOnly );
    EventObj_SSL( int fd );
    virtual bool inp();

protected:
    // to be redefined (if default inp is used)
    virtual int parse( char *beg, char *end ) = 0; ///< return CNT if we need more to read. WAIT permits to keep the connection alive

    virtual bool cnt_default_value() const;
};

#endif // EVENTOBJ_SSL_H
