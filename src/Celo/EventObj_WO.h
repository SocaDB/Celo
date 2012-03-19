#ifndef EVENTOBJ_WO_H
#define EVENTOBJ_WO_H

#include "EventObj_WP.h"

/**
  EventObj with helper methods for input / output of data
*/
class EventObj_WO : public EventObj_WP {
public:
    EventObj_WO( int fd );
    virtual ~EventObj_WO();

    // send data to fd
    void send_cst( const char *data, ST size, bool end = true ); ///< from string data, data + size
    void send_cst( const char *data ); ///< from c_str data

    void send_fid( int src, ST off, ST len ); ///< from file id src

protected:
    virtual bool out();

private:
    void append( struct RemOutput *rem_out );
    void cl_rem();

    struct RemOutput *prim_rem_out;
    struct RemOutput *last_rem_out;
};

#endif // EVENTOBJ_WO_H
