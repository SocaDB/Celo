#ifndef LISTENER_H
#define LISTENER_H

#include "EventObj.h"

/**
 */
class Listener : public EventObj {
public:
    Listener( const char *port );
    Listener( VtableOnly );

    virtual bool inp();

    // called after an accept
    virtual EventObj *event_obj_factory( int fd ) = 0;
};


#endif // LISTENER_H
