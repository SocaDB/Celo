#ifndef Celo_Events_BufferedConnection_WO_H
#define Celo_Events_BufferedConnection_WO_H

#include "BufferedConnection.h"
#include "../System/VoidStruct.h"

namespace Celo {
namespace Events {

/**
  Event_obj which calls obj->parse( beg, end ) each time new data comes in

*/
template<class ObjWithParse,class AdditionalData=VoidStruct,bool del=false>
class BufferedConnection_WO : public BufferedConnection {
public:
    BufferedConnection_WO( ObjWithParse *obj, int fd, AdditionalData data = AdditionalData() ) : BufferedConnection( fd ), data( data ), obj( obj ) {
    }

    ~BufferedConnection_WO() {
        if ( del )
            delete obj;
    }

    virtual bool parse( Ptr<Buffer> buff ) {
        return obj->parse( this, buff );
    }

    AdditionalData data;

protected:
    ObjWithParse *obj;
};

} // namespace Events
} // namespace Celo

#endif // Celo_Events_BufferedConnection_WO_H
