/*
 Copyright 2012 Structure Computation  www.structure-computation.com
 Copyright 2012 Hugo Leclerc

 This file is part of Celo.

 Celo is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Celo is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with Celo. If not, see <http://www.gnu.org/licenses/>.
*/


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
