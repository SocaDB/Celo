#ifndef REQSTATE_NEXTSPACE_H
#define REQSTATE_NEXTSPACE_H

#include "StringBlk.h"
#include "ReqState.h"

/**
*/
class ReqState_NextSpace : public ReqState {
public:
    ReqState_NextSpace( StringBlk &res ) : res( res ) {}

    virtual ReqState *inp( char *data, ST size ) {
        for( int i = 0; i < size; ++i ) {
            if ( data[ i ] == ' ' ) {
                data[ i ] = 0;
                return next;
            }
        }
        return this;
    }

    StringBlk &res;
};

#endif // REQSTATE_NEXTSPACE_H
