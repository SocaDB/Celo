#include "Util/StringHelp.h"
#include "Filtered.h"
#include "Filter.h"
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

namespace Celo {

Filtered::Filtered( int fd, Filter *filter ) : EventObj( fd ), filter( filter ) {
}

Filtered::Filtered( VtableOnly vo ) : EventObj( vo ) {
}

void Filtered::write_cst( const char *data, ST size, bool end ) {
    // send data to filter
    filter->new_out( data, size );

    // send filtered data to the socket
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = filter->get_out( buff, size_buff ); // SSL_read
        if ( ruff == 0 )
            break; // done
        if ( ruff == Filter::NEED_MORE_INPUT )
            return; // wait for more data
        if ( ruff < 0 )
            TODO; // ERROR

        _send_unfiltered( buff, ruff, end and ruff < size_buff );
    }
}

void Filtered::write_cst( const char *data ) {
    return write_cst( data, strlen( data ), false );
}

void Filtered::write_str( const char *data, ST size, bool end ) {
    return write_cst( data, size, end );
}

void Filtered::write_str( const char *data ) {
    return write_str( data, strlen( data ), false );
}

void Filtered::write_fdd( int fd, ST off, ST len ) {
    TODO;
}

void Filtered::wait_for_another_write() {
    TODO;
}

bool Filtered::inp() {
    // send data to filter (fill rbio in the case of Filter_SSL)
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff <= 0 ) {
            // end of available data (but not end of the connection)
            if ( ruff < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                break;
            err();
            return false;
        }

        filter->new_inp( buff, ruff );
    }

    // try to read the filtered data
    while ( true ) {
        ST ruff = filter->get_inp( buff, size_buff ); // SSL_read

        //
        if ( ruff == 0 )
            return true;

        // filter does not give any input :(... maybe there's no enough data
        // and maybe we have to flush output and wait for more (e.g. if negociation)
        if ( ruff == Filter::NEED_MORE_INPUT ) {
            // flush output
            while ( true ) {
                ST ruff = filter->get_out( buff, size_buff );
                if ( ruff == 0 )
                    break;
                if ( ruff < 0 ) {
                    if ( ruff == Filter::NEED_MORE_INPUT )
                        return true;
                    // error
                    err();
                    return false;
                }

                //
                _send_unfiltered( buff, ruff, ruff < size_buff );
            }

            // wait for new input (e.g. the answer) from the client
            return true;
        }

        //
        if ( ruff < 0 ) {
            err();
            return false;
        }

        //
        if ( not parse( buff, buff + ruff ) )
            return false;
    }
}

bool Filtered::out() {
    TODO;
}

void Filtered::_send_unfiltered( const char *data, ST size, bool end ) {
//    if ( not size )
//        return;

//    if ( still_has_something_to_send() )
//        return append( new T( data, size, end ) );

//    while ( true ) {
        ST real = ::send( fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
//        if ( real <= 0 ) { // error ?
//            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
//                return append( new T( data, size, end ) );
//            cl_rem_and_add_err();
//            return err();
//        }

//        size -= real;
//        if ( not size ) // done ?
//            return;
//        data += real;
//    }
}

}

