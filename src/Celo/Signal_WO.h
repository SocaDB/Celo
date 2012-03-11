#ifndef SIGNAL_WO_H
#define SIGNAL_WO_H

#include "Signal.h"

/**
*/
template<class ObjWithSignal>
class Signal_WO : public Signal {
public:
    Signal_WO( ObjWithSignal *obj, const int *sigs ) : Signal( sigs ), obj( obj ) {}
    virtual bool signal( int s ) { return obj->signal( s ); }

protected:
    ObjWithSignal *obj;
};



#endif // SIGNAL_WO_H
