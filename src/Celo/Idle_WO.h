#ifndef IDLE_WO_H
#define IDLE_WO_H

#include "IdleObj.h"

/**
*/
template<class ObjWithIdle>
class Idle_WO : public IdleObj {
public:
    Idle_WO( ObjWithIdle *obj ) : obj( obj ) {}
    virtual bool inp() { return obj->idle(); }

protected:
    ObjWithIdle *obj;
};

#endif // IDLE_WO_H
