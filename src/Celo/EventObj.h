#ifndef EVENTOBJ_H
#define EVENTOBJ_H

#include "Config.h"

/**
*/
class EventObj {
public:
    EventObj( int fd );
    virtual ~EventObj();

    virtual bool inp(); ///< if input data. return true if needs or may accept more.
    virtual bool out(); ///< if ready for output. return true if there is still remainging data to send.
    virtual void err(); ///< if error
    virtual void hup(); ///< if closed
    virtual void rdy(); ///< called after installation of this in event loop

    virtual bool want_poll_inp_at_the_beginning() const;
    virtual bool want_poll_out_at_the_beginning() const;

    void poll_out(); ///< add output polling for $this in ev_loop

    class EventLoop *ev_loop;
    int              fd;
};

#endif // EVENTOBJ_H
