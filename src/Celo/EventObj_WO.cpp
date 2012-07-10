#include "StringHelp.h"
#include "RemOutput.h"
#include "EventLoop.h"

template<class T>
static void _send( EventObj_WO *eo, const char *data, ST size, bool end ) {
    while ( true ) {
        ST real = ::send( eo->fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
        //std::cout.write( data, size );
        if ( real < 0 ) { // error ?
            if ( errno == EAGAIN )
                continue;
            return eo->cl_rem();
        }

        size -= real;
        if ( not size ) // done ?
            return;

        if ( not real ) ///< there's remaining data but we have to wait for the next "out round"
            return eo->append( new T( data, size, end ) );
        data += real;
    }
}

EventObj_WO::EventObj_WO( int fd ) : EventObj_WP( fd ), prim_rem_out( 0 ), last_rem_out( 0 ) {
}

EventObj_WO::EventObj_WO( VtableOnly vo ) : EventObj_WP( vo ) {
}

EventObj_WO::~EventObj_WO() {
    cl_rem();
}

bool EventObj_WO::out() {
    while ( prim_rem_out ) {
        if ( not prim_rem_out->write( this ) )
            return true;

        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
    last_rem_out = 0;
    return false;
}

void EventObj_WO::send_cst( const char *data, ST size, bool end ) {
    out(); // if we have something to send first
    _send<RemOutputData>( this, data, size, end );
}

void EventObj_WO::send_cst( const char *data ) {
    send_cst( data, strlen( data ) );
}

void EventObj_WO::send_str( const char *data, ST size, bool end ) {
    out(); // if we have something to send first
    _send<RemOutputDataCopy>( this, data, size, end );
}

void EventObj_WO::send_str( const char *data ) {
    send_str( data, strlen( data ) );
}

void EventObj_WO::send_fid( int src, ST off, ST size ) {
    out(); // if we have something to send first
    off_t offset = off;
    while ( true ) {
        ssize_t real = sendfile( fd, src, &offset, size );
        if ( real < 0 ) {
            if ( errno == EAGAIN )
                continue;
            return cl_rem();
        }

        size -= real;
        if ( not size ) { // done ?
            close( src );
            return;
        }

        if ( not real ) ///< there's remaining data but we have to wait for the next "out round"
            return append( new RemOutputFile( src, offset, size ) );
    }
}

void EventObj_WO::wait_for_send() {
    append( new RemOutputWait );
}

bool EventObj_WO::still_has_something_to_send() const {
    return prim_rem_out;
}

void EventObj_WO::append( RemOutput *rem_out ) {
    rem_out->next = 0;

    if ( last_rem_out )
        last_rem_out->next = rem_out;
    last_rem_out = rem_out;

    if ( not prim_rem_out ) {
        prim_rem_out = rem_out;
        ev_loop->mod( this, true );
    }
}

void EventObj_WO::cl_rem() {
    while( prim_rem_out ) {
        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
}
