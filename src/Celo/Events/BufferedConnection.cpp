#include "BufferedConnection.h"
#include "Internal/RemOutput.h"
#include <string.h>

#include "../Util/StringHelp.h"

namespace Celo {
namespace Events {

BufferedConnection::BufferedConnection( VtableOnly vo ) : Event( vo ) {
}

BufferedConnection::BufferedConnection( int fd ) : Event( fd ), prim_rem_out( 0 ), last_rem_out( 0 ) {
}

BufferedConnection::~BufferedConnection() {
    cl_rem();
}


template<class T>
void BufferedConnection::_write( const char *data, ST size, bool end ) {
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
void BufferedConnection::_write( int src, ST offset, ST size, bool end ) {
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

void BufferedConnection::write_cst( const char *data, ST size, bool end ) {
    _write<RemOutputData>( data, size, end );
}

void BufferedConnection::write_cst( const char *data ) {
    return write_cst( data, strlen( data ) );
}

void BufferedConnection::write_str( const char *data, ST size, bool end ) {
    _write<RemOutputCopiedData>( data, size, end );
}

void BufferedConnection::write_str( const char *data ) {
    return write_str( data, strlen( data ) );
}

void BufferedConnection::write_fdd( int fd, ST off, ST len ) {
    _write<RemOutputFile>( fd, off, len, true );
}

void BufferedConnection::wait_for_another_write() {
    append( new RemOutputWait );
}

bool BufferedConnection::still_has_something_to_send() const {
    return prim_rem_out;
}

bool BufferedConnection::flush() {
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

bool BufferedConnection::inp() {
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff <= 0 ) {
            // need a retry ?
            if ( ruff < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                return true;
            err();
            return false;
        }

        // parse
        if ( not parse( buff, buff + ruff ) )
            return false;
    }
}


bool BufferedConnection::out() {
    flush();
    return true;
}

void BufferedConnection::cl_rem_and_add_err() {
    cl_rem();
    append( new RemOutputError() );
}

void BufferedConnection::append( RemOutput *rem_out ) {
    rem_out->next = 0;

    if ( last_rem_out )
        last_rem_out->next = rem_out;
    last_rem_out = rem_out;

    if ( not prim_rem_out ) {
        prim_rem_out = rem_out;
        poll_out();
    }
}

void BufferedConnection::cl_rem() {
    while( prim_rem_out ) {
        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
}

} // namespace Events
} // namespace Celo
