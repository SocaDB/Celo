#ifndef EVENTOBJ_WO_H
#define EVENTOBJ_WO_H

#include "EventObj.h"

/**
  EventObj with helper methods for input / output of data
*/
class EventObj_WO : public EventObj {
public:
    EventObj_WO( int fd );
    virtual ~EventObj_WO();

    // send data to fd
    void send( const char *data, ST size, bool end = true ); ///< from string data, data + size
    void send( int src, ST off, ST len ); ///< from file src

protected:
    virtual bool inp();
    virtual bool out();

    // to be redefined (if default inp is used)
    virtual bool inp( char *beg, char *end ) = 0; ///< return true if we need more to read

private:
    void append( struct RemOutput *rem_out );
    void cl_rem();

    struct RemOutput *prim_rem_out;
    struct RemOutput *last_rem_out;
};

#endif // EVENTOBJ_WO_H
