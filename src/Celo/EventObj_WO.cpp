#include "RemOutput.h"
#include "StringHelp.h"

EventObj_WO::EventObj_WO( int fd ) : EventObj( fd ), prim_rem_out( 0 ), last_rem_out( 0 ) {
}

EventObj_WO::~EventObj_WO() {
    cl_rem();
}


bool EventObj_WO::inp() {
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff < 0 ) {
            if ( errno == EAGAIN )
                continue;
            return false;
        }

        // we need more data but we don't have ?
        if ( ruff == 0 )
            return true;

        // parse
        // write( 0, buff, ruff );
        if ( not inp( buff, buff + ruff ) )
            return false;
    }
}

bool EventObj_WO::out() {
    while ( prim_rem_out ) {
        if ( not prim_rem_out->write( this ) )
            return true;

        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
    return false;
}

void EventObj_WO::send( const char *data, ST size, bool end ) {
    ST real = ::send( fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
    if ( real < 0 )
        return cl_rem();

    size -= real;
    if ( size > 0 ) {
        data += real;
        append( new RemOutputData( data, size, end ) );
    }
}

void EventObj_WO::send( int src, ST off, ST len ) {
    off_t offset = off;
    ssize_t size = sendfile( fd, src, &offset, len );
    if ( size < 0 ) {
        if ( errno == EAGAIN )
            return append( new RemOutputFile( src, offset, len ) );
        return cl_rem();
    }

    len -= size;
    if ( len )
        return append( new RemOutputFile( src, offset, len ) );

    close( src );
}

void EventObj_WO::append( RemOutput *rem_out ) {
    rem_out->next = 0;

    if ( last_rem_out )
        last_rem_out->next = rem_out;
    last_rem_out = rem_out;

    if ( not prim_rem_out ) {
        prim_rem_out = rem_out;
        //
        poll_out();
    }
}

void EventObj_WO::cl_rem() {
    while( prim_rem_out ) {
        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
}
