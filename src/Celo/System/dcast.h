#ifndef DCAST_H
#define DCAST_H

template<class T>
struct DCast {
    DCast( T *p ) : p( p ) {
    }
    template<class O> operator O *() const {
        return dynamic_cast<O *>( p );
    }
    T *p;
};

template<class T>
DCast<T> dcast( T *p ) {
    return p;
}

#endif // DCAST_H
