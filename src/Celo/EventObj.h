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

    virtual bool inp(); ///< if input data
    virtual bool out(); ///< if ready for output
    virtual void err(); ///< if error
    virtual void hup(); ///< if closed
    virtual void rdy(); ///< called after installation of this in event loop

    bool send( const char *data, ST size, bool end = true );
    bool send( const std::string &str, bool end = true );

    class EventLoop  *ev_loop;
    class RemOutData *rem_out;
    int               fd;
};

#endif // EVENTOBJ_H
