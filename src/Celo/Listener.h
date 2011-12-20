#ifndef LISTENER_H
#define LISTENER_H

#include "EventObj.h"

/**
 */
class ListenerAncestor : public EventObj {
public:
    ListenerAncestor( const char *port );
    virtual bool inp();

    virtual EventObj *event_obj_factory( int fd ) = 0;
};

/**
*/
template<class ObjWithEventObjFactory>
class Listener : public ListenerAncestor {
public:
    Listener( ObjWithEventObjFactory *obj, const char *port ) : obj( obj ), ListenerAncestor( port ) {
    }

    virtual EventObj *event_obj_factory( int fd ) {
        return obj->event_obj_factory( fd );
    }

    ObjWithEventObjFactory *obj;
};

#endif // LISTENER_H
