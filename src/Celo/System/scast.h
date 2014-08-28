#ifndef SCAST_H
#define SCAST_H

template<class T>
struct SCast {
    SCast( T *p ) : p( p ) {
    }
    template<class O> operator O *() const {
        return static_cast<O *>( p );
    }
    T *p;
};

template<class T>
SCast<T> scast( T *p ) {
    return p;
}

template<class T>
struct SCastC {
    SCastC( const T *p ) : p( p ) {
    }
    template<class O> operator const O *() const {
        return static_cast<const O *>( p );
    }
    const T *p;
};

template<class T>
SCastC<T> scast( const T *p ) {
    return p;
}

#endif // SCAST_H
