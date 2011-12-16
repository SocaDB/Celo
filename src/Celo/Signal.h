#ifndef SIGNAL_H
#define SIGNAL_H

#include "EventObj.h"

/**
*/
class SignalAncestor : public EventObj {
public:
    SignalAncestor( const int *sigs );
    virtual bool inp();

    virtual bool signal( int s ) = 0;
};


/**
*/
template<class ObjWithSignal>
class Signal : public SignalAncestor {
public:
    Signal( ObjWithSignal *obj, const int *sigs ) : SignalAncestor( sigs ), obj( obj ) {}
    virtual bool signal( int s ) { return obj->signal( s ); }

protected:
    ObjWithSignal *obj;
};

#endif // SIGNAL_H
