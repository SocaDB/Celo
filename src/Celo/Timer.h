#ifndef TIMER_H
#define TIMER_H

#include "EventObj.h"

/**
*/
class Timer : public EventObj {
public:
    Timer( double delay );
    virtual bool inp();

    virtual void timeout() = 0;
};

#endif // TIMER_H
