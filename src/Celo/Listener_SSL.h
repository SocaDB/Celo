#ifndef LISTENER_SSL_H
#define LISTENER_SSL_H

#include "EventObj.h"

/**
*/
class Listener_SSL : public EventObj {
public:
    Listener_SSL( const char *port );
    Listener_SSL( VtableOnly );

    virtual EventObj *event_obj_factory( int fd );
};

#endif // LISTENER_SSL_H
