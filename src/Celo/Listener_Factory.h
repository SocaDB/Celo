#ifndef LISTENER_FACTORY_H
#define LISTENER_FACTORY_H

#include "Listener_WithLaunch.h"

/**
*/
template<class EO,class PR = void,class PA = Listener_WithLaunch>
class Listener_Factory : public PA {
public:
    Listener_Factory( const char *port, PR *pr = 0 ) : PA( port ), pr( pr ) {}
    Listener_Factory( typename PA::VtableOnly vo ) : PA( vo ) {}

    virtual EventObj *event_obj_factory( int fd ) {
        return new EO( fd, pr );
    }

    PR *pr;
};

#endif // LISTENER_FACTORY_H
