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

    SSL_set_accept_state( ssl );
    SSL_set_bio( ssl, rbio, wbio );
}

EventObj_WO_SSL::EventObj_WO_SSL( VtableOnly vo ) : EventObj( vo ) {
    SSL_free( ssl );
}

bool EventObj_WO_SSL::inp() {
    PRINT( "inp" );
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff < 0 ) {
            // EAGAIN
            if ( errno == EAGAIN ) {
                out();
                return true;
            }
            return false;
        }

        // if we need more data and
        if ( ruff == 0 )
            return true;

        // filter
        ST written = BIO_write( rbio, buff, ruff );
        if ( written != ruff )
            TODO;
        PRINT( written );

        // parse
        while ( true ) {
            const int ns = 1024;
            char data[ ns ];
            int read = SSL_read( ssl, data, ns );

            PRINT( read );
            if ( read > 0 ) {
                write( 0, data, read );
                int p = parse( data, data + read );

                if ( p )
                    return p < 0;
            } else if ( not _continue_ssl( read ) )
                TODO;

            if ( read != ns or written == 0 )
                break;
        }
    }

    out();
}

bool EventObj_WO_SSL::out() {
    PRINT( "out" );
    while ( true ) {
        const int ds = 1024;
        char data[ ds ];
        int size = BIO_read( wbio, data, ds );
        PRINT( size );

        if ( size > 0 )
            ::send( fd, data, size, MSG_NOSIGNAL );

        if ( size != ds || size == 0 )
            break;
    }
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
