#ifndef CELO_PARSABLE_WO_H
#define CELO_PARSABLE_WO_H

#include "VoidStruct.h"
#include "Parsable.h"

namespace Celo {

/**
  Event_obj which calls obj->parse( beg, end ) each time new data comes in

*/
template<class ObjWithSignal,class AdditionalData=VoidStruct,bool del=false>
class Parsable_WO : public Parsable {
public:
    Parsable_WO( ObjWithSignal *obj, int fd, AdditionalData data = AdditionalData() ) : Parsable( fd ), data( data ), obj( obj ) {
    }

    ~Parsable_WO() {
        if ( del )
            delete obj;
    }

    virtual bool parse( char *beg, char *end ) {
        return obj->parse( this, beg, end );
    }

    AdditionalData data;

protected:
    ObjWithSignal *obj;
};

}

#endif // CELO_PARSABLE_WO_H
