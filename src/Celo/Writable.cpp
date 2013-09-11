#include "Util/StringHelp.h"
#include "Util/RemOutput.h"
#include "Writable.h"
#include <string.h>

namespace Celo {

Writable::Writable( VtableOnly vo ) : EventObj( vo ) {
}

Writable::Writable( int fd ) : EventObj( fd ), prim_rem_out( 0 ), last_rem_out( 0 ) {
}

Writable::~Writable() {
    cl_rem();
}


template<class T>
void Writable::_write( const char *data, ST size, bool end ) {
    if ( not size )
        return;

    if ( still_has_something_to_send() )
        return append( new T( data, size, end ) );

    while ( true ) {
        ST real = ::send( fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
        if ( real <= 0 ) { // error ?
            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                return append( new T( data, size, end ) );
            cl_rem_and_add_err();
            return err();
        }

        size -= real;
        if ( not size ) // done ?
            return;
        data += real;
    }
}

template<class T>
void Writable::_write( int src, ST offset, ST size, bool end ) {
    if ( not size )
        return;

    if ( still_has_something_to_send() )
        return append( new T( src, offset, size ) );

    while ( true ) {
        ST real = sendfile( fd, src, &offset, size );
        if ( real <= 0 ) { // error ?
            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                return append( new T( src, offset, size ) );
            cl_rem_and_add_err();
            close( src );
            return err();
        }

        size -= real;
        if ( not size ) { // done ?
            close( src );
            return;
        }
    }
}

void Writable::write_cst( const char *data, ST size, bool end ) {
    _write<RemOutputData>( data, size, end );
}

void Writable::write_cst( const char *data ) {
    return write_cst( data, strlen( data ) );
}

void Writable::write_str( const char *data, ST size, bool end ) {
    _write<RemOutputCopiedData>( data, size, end );
}

void Writable::write_str( const char *data ) {
    return write_str( data, strlen( data ) );
}

void Writable::write_fdd( int fd, ST off, ST len ) {
    _write<RemOutputFile>( fd, off, len, true );
}

void Writable::wait_for_another_write() {
    append( new RemOutputWait );
}

bool Writable::still_has_something_to_send() const {
    return prim_rem_out;
}

bool Writable::flush() {
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

bool Writable::out() {
    flush();
    return true;
}

void Writable::cl_rem_and_add_err() {
    cl_rem();
    append( new RemOutputError() );
}

void Writable::append( RemOutput *rem_out ) {
    rem_out->next = 0;

    if ( last_rem_out )
        last_rem_out->next = rem_out;
    last_rem_out = rem_out;

    if ( not prim_rem_out ) {
        prim_rem_out = rem_out;
        poll_out();
    }
}

void Writable::cl_rem() {
    while( prim_rem_out ) {
        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
}

}
