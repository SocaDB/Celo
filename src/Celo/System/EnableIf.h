#ifndef ENABLEIF_H
#define ENABLEIF_H

namespace Celo {

template<int cond,class T1=void,class T2=void,class T3=void>
struct EnableIf {};

template<class T1,class T2,class T3>
struct EnableIf<1,T1,T2,T3> {
    typedef T1 T;
};

template<class T1,class T2,class T3>
struct EnableIf<2,T1,T2,T3> {
    typedef T2 T;
};

template<class T1,class T2,class T3>
struct EnableIf<3,T1,T2,T3> {
    typedef T3 T;
};

struct Nawak {};

}

#endif // ENABLEIF_H
