#ifndef SIGNAL_H
#define SIGNAL_H

#include "EventObj.h"

/**
*/
class Signal : public EventObj {
public:
    Signal( const int *sigs );
    virtual bool inp();

    virtual bool signal( int s ) = 0;
};


#endif // SIGNAL_H
