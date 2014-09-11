#include "BufferedConnection.h"
#include "Internal/RemOutput.h"
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

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
            return hup_error();
        }

        size -= real;
        if ( not size ) // done ?
            return;
        data += real;
    }
}

template<class T>
void BufferedConnection::_write( int src, off_t offset, ST size, bool end ) {
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
            return hup_error();
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

void BufferedConnection::write_buf( Ptr<Buffer> buf, int off, bool end ) {
    while ( off >= buf->used ) {
        off -= buf->used;
        buf = buf->next;
        if ( not buf )
            return;
    }

    if ( still_has_something_to_send() )
        return append( new RemOutputBuffer( buf, off, end ) );

    while ( true ) {
        ST real = ::send( fd, buf->data + off, buf->used - off, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
        if ( real <= 0 ) { // error ?
            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                return append( new RemOutputBuffer( buf, off, end ) );
            cl_rem_and_add_err();
            return hup_error();
        }

        off += real;
        if ( off >= buf->used ) {
            while ( true ) {
                buf = buf->next;
                if ( not buf )
                    return;
                if ( buf->used )
                    break;
            }
            off = 0;
        }
    }
}

void BufferedConnection::wait_for_another_write() {
    append( new RemOutputWait );
}

bool BufferedConnection::still_has_something_to_send() const {
    return prim_rem_out;
}

void BufferedConnection::inp() {
    while ( true ) {
        Ptr<Buffer> buff = new Buffer;
        buff->used = read( fd, buff->data,  buff->item_size );
        if ( buff->used <= 0 ) {
            // need a retry or there are more data to come
            if ( buff->used < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                return wait_for_more_inp();
            return hup_error();
        }

        // parse
        if ( not parse( buff ) )
            return;
    }
}


void BufferedConnection::out() {
    while ( prim_rem_out ) {
        if ( not prim_rem_out->write( this ) )
            return wait_for_more_out( false );

        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
    last_rem_out = 0;
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

    wait_for_more_out( false );
}

void BufferedConnection::cl_rem() {
    while ( prim_rem_out ) {
        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
}

} // namespace Events
} // namespace Celo
