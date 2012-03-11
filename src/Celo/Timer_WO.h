#ifndef TIMER_WO_H
#define TIMER_WO_H

#include "Timer.h"

/**
*/
template<class ObjWithTimeout>
class Timer_WO : public Timer {
public:
    Timer_WO( ObjWithTimeout *obj, double delay ) : Timer( delay ), obj( obj ) {}
    virtual void timeout() { obj->timeout(); }

protected:
    ObjWithTimeout *obj;
};

#endif // TIMER_WO_H
