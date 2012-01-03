#ifndef EVENTOBJ_H
#define EVENTOBJ_H

#include "Config.h"
#include <string>

/**
*/
class EventObj {
public:
    EventObj( int fd );
    virtual ~EventObj();

    virtual void inp(); ///< if input data.
    virtual void out(); ///< if ready for output.
    virtual void err(); ///< if error
    virtual void hup(); ///< if closed
    virtual void rdy(); ///< called after installation of this in event loop
    virtual bool end(); ///< if input data

    class EventLoop *ev_loop;
    int              fd;
};

#endif // EVENTOBJ_H
