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


#ifndef REMOUTPUT_H
#define REMOUTPUT_H

#include "EventObj_WO.h"
#include "StringHelp.h"

#include <sys/sendfile.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/**
*/
struct RemOutput {
    virtual bool write( EventObj_WO *obj ) = 0; ///< return true if done
    virtual ~RemOutput() {}
    RemOutput *next;
};

/**
*/
struct RemOutputData : public RemOutput {
    RemOutputData( const char *data, ST size, bool end ) : data( data ), size( size ), end( end ) {
    }

    virtual bool write( EventObj_WO *obj ) {
        while ( true ) {
            ST real = ::send( obj->fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
            if ( real < 0 ) { // error ?
                if ( errno == EAGAIN )
                    continue;
                return true;
            }

            size -= real;
            if ( not size ) // finished ?
                return true;

            if ( not real ) ///< there's remaining data but we have to wait for the next "out round"
                return false;
            data += real;
        }
    }

    const char *data;
    ST size;
    bool end; ///< false to send using MSG_MORE
};

/**
*/
struct RemOutputDataCopy : public RemOutputData {
    RemOutputDataCopy( const char *dat_, ST size, bool end ) : RemOutputData( 0, size, end ) {
        orig = strdup( dat_ );
        data = orig;
    }

    virtual ~RemOutputDataCopy() {
        free( orig );
    }

    char *orig;
};

/**
*/
struct RemOutputFile : public RemOutput {
    RemOutputFile( int src, off_t off, ST size ) : src( src ), off( off ), size( size ) {
    }

    virtual ~RemOutputFile() {
        close( src );
    }

    virtual bool write( EventObj_WO *obj ) {
        while ( true ) {
            ST real = sendfile( obj->fd, src, &off, size );
            if ( real < 0 ) { // error ?
                if ( errno == EAGAIN )
                    continue;
                return true;
            }

            size -= real;
            if ( not size ) // finished ?
                return true;

            if ( not real ) ///< there's remaining data but we have to wait for the next "out round"
                return false;
        }
    }

    int   src;
    off_t off;
    ST    size;
};

/**
  say "done" if followed by something to send
*/
struct RemOutputWait : public RemOutput {
    virtual bool write( EventObj_WO *obj ) {
        return next;
    }
};

#endif // REMOUTPUT_H
