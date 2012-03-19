#ifndef STREAMSEP_H
#define STREAMSEP_H

/**

*/
template<class TS>
class StreamSep {
public:
    StreamSep( TS *stream, const char *sep, const char *end ) : stream( stream ), sep( sep ), end( end ) {}
    ~StreamSep() { if ( end ) *stream << end; }

    ///
    template<class T>
    StreamSep &operator<<( const T &val ) {
        if ( sep )
            *stream << sep;
        *stream << val;
        return *this;
    }

    TS *stream;
    const char *sep;
    const char *end;
};


/**

*/
template<class TS>
class StreamSepMaker {
public:
    StreamSepMaker( TS &stream, const char *sep = 0, const char *end = "\n" ) : stream( &stream ), sep( sep ), end( end ), beg( 0 ), first_beg( 0 ) {}

    ///
    template<class T>
    StreamSep<TS> operator<<( const T &val ) {
        if( first_beg ) {
            *stream << first_beg;
            first_beg = 0;
        } else if ( beg )
            *stream << beg;
        *stream << val;
        return StreamSep<TS>( stream, sep, end );
    }

    TS *stream;
    const char *sep;
    const char *end;
    const char *beg;
    const char *first_beg;
};

#endif // STREAMSEP_H
