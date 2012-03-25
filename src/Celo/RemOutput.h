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
    bool end;
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

#endif // REMOUTPUT_H
