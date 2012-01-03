#ifndef EVENTOBJ_WO_H
#define EVENTOBJ_WO_H

#include "EventObj.h"

/**
  EventObj with helper to output data
*/
class EventObj_WO : public EventObj {
public:
    EventObj_WO( int fd );
    virtual ~EventObj_WO();

    virtual void out();
    virtual bool end();

    // send data to fd
    void send( const char *data, ST size, bool end = true ); ///< from string data, data + size
    void send( int src, ST off, ST len ); ///< from file src

    struct RemOutput *prim_rem_out;
    struct RemOutput *last_rem_out;
};

#endif // EVENTOBJ_WO_H
