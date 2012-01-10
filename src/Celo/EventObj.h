#ifndef EVENTOBJ_H
#define EVENTOBJ_H

#include "Config.h"

/**
*/
class EventObj {
public:
    EventObj( int fd );
    virtual ~EventObj();

    virtual bool inp(); ///< if input data. return true if needs of may accept more.
    virtual bool out(); ///< if ready for output. return true if there is still remainging data to send.
    virtual void err(); ///< if error
    virtual void hup(); ///< if closed
    virtual void rdy(); ///< called after installation of this in event loop

    void poll_out(); ///< poll this for output in ev_loop

    class EventLoop *ev_loop;
    int              fd;
};

#endif // EVENTOBJ_H
