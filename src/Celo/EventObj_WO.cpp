#include "EventObj_WO.h"
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct RemOutput {
    RemOutput( RemOutput *old_last ) { old_last->next = this; }
    virtual bool write( EventObj_WO *obj ) = 0;
    virtual ~RemOutput() {}
    RemOutput *next;
};

struct RemOutputData : public RemOutput {
    RemOutputData( RemOutput *old_last, const char *data, ST size, bool end ) : RemOutput( old_last ), data( data ), size( size ), end( end ) {
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

struct RemOutputDataCopy : public RemOutputData {
    RemOutputDataCopy( RemOutput *old_last, const char *dat_, ST size, bool end ) : RemOutputData( old_last, 0, size, end ) {
        orig = strdup( dat_ );
        data = orig;
    }

    virtual ~RemOutputDataCopy() {
        free( orig );
    }

    char *orig;
};

struct RemOutputFile : public RemOutput {
    RemOutputFile( RemOutput *old_last, int src, ST off, ST len ) : RemOutput( old_last ), src( src ), off( off ), len( len ) {
    }

    virtual ~RemOutputFile() {
        close( src );
    }

    virtual bool write( EventObj_WO *obj ) {
        off_t offset = off;
        ssize_t size = sendfile( obj->fd, src, &offset, len );
        len -= size;
        if ( len > 0 ) {
            off += size;
            return false;
        }
        return true;
    }

    int src;
    ST  off;
    ST  len;
};


EventObj_WO::EventObj_WO( int fd ) : EventObj( fd ), prim_rem_out( 0 ), last_rem_out( 0 ) {
}

EventObj_WO::~EventObj_WO() {
}

void EventObj_WO::out() {
    while( prim_rem_out ) {
        if ( not prim_rem_out->write( this ) )
            return;

        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }

    if ( end() )
        delete this;
}

bool EventObj_WO::end() {
    return EventObj::end() and not prim_rem_out;
}

void EventObj_WO::send( const char *data, ST size, bool end ) {
    ST real = ::send( fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
    if ( real < 0 ) {
        delete this;
        return;
    }

    if ( ( size -= real ) > 0 ) {
        data += real;
        last_rem_out = new RemOutputData( last_rem_out, data, size, end );
    }
}

void EventObj_WO::send( int src, ST off, ST len ) {
    off_t offset = off;
    ssize_t size = sendfile( fd, src, &offset, len );
    if ( size < len )
        last_rem_out = new RemOutputFile( last_rem_out, src, off + size, len - size );
    else
        close( src );
}
