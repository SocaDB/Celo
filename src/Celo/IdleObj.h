#ifndef IDLEOBJ_H
#define IDLEOBJ_H

/**
*/
class IdleObj {
public:
    IdleObj();
    virtual ~IdleObj();

    virtual bool inp() = 0; ///< if input data. return true to continue (keep the object)

    IdleObj *prev_idle;
};

#endif // IDLEOBJ_H
