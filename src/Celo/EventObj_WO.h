#ifndef EVENTOBJ_WO_H
#define EVENTOBJ_WO_H

#include "EventObj_WP.h"

/**
  EventObj with helper methods for input / output of data
*/
class EventObj_WO : public EventObj_WP {
public:
    EventObj_WO( VtableOnly );
    EventObj_WO( int fd );
    virtual ~EventObj_WO();

    // send data to fd
    void send_cst( const char *data, ST size, bool end = true ); ///< permanent data (no copy, no free)
    void send_cst( const char *data ); ///< permanent data (no copy, no free)

    void send_str( const char *data, ST size, bool end = true ); ///< data will be copied (if not sent in one shot) and freed
    void send_str( const char *data ); ///< data will be copied (if not sent in one shot) and freed

    void send_fid( int src, ST off, ST len ); ///< from file id src

    void wait_for_send(); ///< wait until there is something to send (that is a send_... is issued)

protected:
    virtual bool out();

private:
    template<class T> friend void _send( EventObj_WO *eo, const char *data, ST size, bool end );
    void append( struct RemOutput *rem_out );
    void cl_rem();

    struct RemOutput *prim_rem_out;
    struct RemOutput *last_rem_out;
};

#endif // EVENTOBJ_WO_H
