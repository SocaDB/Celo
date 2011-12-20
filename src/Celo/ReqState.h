#ifndef REQSTATE_H
#define REQSTATE_H

#include "Config.h"

/**
*/
class ReqState {
public:
    ReqState() : next( next ) {}

    virtual ReqState *inp( class HttpRequest *, char *data, ST size ) = 0;

    ReqState *next;
};

#endif // REQSTATE_H
