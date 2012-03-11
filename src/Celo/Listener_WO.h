#ifndef LISTENER_WO_H
#define LISTENER_WO_H

#include "Listener.h"

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

protected:
    ObjWithEventObjFactory *obj;
};


#endif // LISTENER_WO_H
