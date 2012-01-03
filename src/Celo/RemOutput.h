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
    virtual bool write( EventObj_WO *obj ) = 0;
    virtual ~RemOutput() {}
    RemOutput *next;
};

/**
*/
struct RemOutputData : public RemOutput {
    RemOutputData( const char *data, ST size, bool end ) : data( data ), size( size ), end( end ) {
    }

    virtual bool write( EventObj_WO *obj ) {
        ST real = ::send( obj->fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
        if ( real < 0 ) {
            delete obj;
            return false;
        }
        size -= real;
        if ( size > 0 ) {
            data += real;
            return false;
        }
        return true;
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
    RemOutputFile( int src, off_t off, ST len ) : src( src ), off( off ), len( len ) {
    }

    virtual ~RemOutputFile() {
        close( src );
    }

    virtual bool write( EventObj_WO *obj ) {
        ssize_t size = sendfile( obj->fd, src, &off, len );
        if ( size < 0 )
            return errno != EAGAIN;
        if ( not size )
            return false;

        len -= size;
        return not len;
    }

    int   src;
    off_t off;
    ST    len;
};

#endif // REMOUTPUT_H
