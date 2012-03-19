#ifndef LISTENER_FACTORY_H
#define LISTENER_FACTORY_H

#include "Listener_WithLaunch.h"

/**
*/
template<class EO,class PR = void,class PA = Listener_WithLaunch>
class Listener_Factory : public PA {
public:
    Listener_Factory( const char *port, PR *pr = 0 ) : Listener_WithLaunch( port ), pr( pr ) {
    }

    virtual EventObj *event_obj_factory( int fd ) {
        return new EO( fd, pr );
    }

    PR *pr;
};

#endif // LISTENER_FACTORY_H
