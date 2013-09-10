#include "EventObj_WO_SSL.h"
#include "StringHelp.h"
#include <unistd.h>
#include <errno.h>

#include <sys/sendfile.h>
#include <sys/socket.h>


EventObj_WO_SSL::EventObj_WO_SSL( SSL_CTX *ctx, int fd ) : EventObj( fd ), ctx( ctx ) {
    rbio = BIO_new( BIO_s_mem() );
    wbio = BIO_new( BIO_s_mem() );

    ssl = SSL_new( ctx );

    SSL_set_mode( ssl, SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER );

    SSL_set_accept_state( ssl );
    SSL_set_bio( ssl, rbio, wbio );
}

EventObj_WO_SSL::EventObj_WO_SSL( VtableOnly vo ) : EventObj( vo ) {
    SSL_free( ssl );
}

bool EventObj_WO_SSL::inp() {
    // copy data from socket to rbio
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff < 0 ) {
            // EAGAIN
            if ( errno == EAGAIN )
                return true;
            return false;
        }

        // no data have been provided -> wait for next signal
        if ( ruff == 0 )
            return true;

        // copy to openssl rbio
        ST written = BIO_write( rbio, buff, ruff );
        if ( written != ruff ) {
            TODO;
            abort();
        }

        // SSL <-> socket (using local copy)
        while ( true ) {
            // get unencrypted data
            int read = SSL_read( ssl, buff, size_buff );
            PRINT( read );
            switch ( SSL_get_error( ssl, read ) ) {
            case SSL_ERROR_NONE:
                if ( int p = parse( buff, buff + read ) )
                    return p < 0;
                break;
            case SSL_ERROR_ZERO_RETURN:
                // end of the connection
                return false;
            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
                PRINT( "w" );
                // continue
                break;
            default:
                // a real error
                return false;
            }

            // send some data to socket if necessary
            int size = BIO_read( wbio, buff, size_buff );
            PRINT( size );
//            if ( size < 0 )
//                break;
            if ( size > 0 )
                send_str( buff, size, false );
        }
    }
}

bool EventObj_WO_SSL::out() {
    PRINT( "out" );
}

void EventObj_WO_SSL::send_cst( const char *data, ST size, bool end ) {
    ST written = SSL_write( ssl, data, size );
    if ( not _continue_ssl() )
        TODO;
    if ( written != size )
        TODO;

    out();
}

void EventObj_WO_SSL::send_cst( const char *data ) {
    send_cst( data, strlen( data ) );
}

void EventObj_WO_SSL::send_str( const char *data, ST size, bool end ) {
    send_cst( data, size, end );
}

void EventObj_WO_SSL::send_str( const char *data ) {
    send_str( data, strlen( data ) );
}

void EventObj_WO_SSL::send_fid( int src, ST off, ST size ) {
    //    out(); // if we have something to send first
    //    off_t offset = off;
    //    while ( true ) {
    //        ssize_t real = sendfile( fd, src, &offset, size );
    //        if ( real < 0 ) {
    //            if ( errno == EAGAIN )
    //                continue;
    //            return cl_rem();
    //        }

    //        size -= real;
    //        if ( not size ) { // done ?
    //            close( src );
    //            return;
    //        }

    //        if ( not real ) ///< there's remaining data but we have to wait for the next "out round"
    //            return append( new RemOutputFile( src, offset, size ) );
    //    }
    TODO;
}

bool EventObj_WO_SSL::cnt_default_value() const {
    return false;
}


bool EventObj_WO_SSL::_continue_ssl( int function_return ) {
    int err = SSL_get_error( ssl, function_return );

    if ( err == SSL_ERROR_NONE or err == SSL_ERROR_WANT_READ )
        return true;

    if ( err == SSL_ERROR_SYSCALL ) {
        ERR_print_errors_fp( stdout );
        perror( "syscall error: " );
        return false;
    }

    if ( err == SSL_ERROR_SSL ) {
        ERR_print_errors_fp( stderr );
        return false;
    }

    return true;
}
