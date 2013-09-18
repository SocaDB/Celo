#include "SslBufferedConnection.h"
#include "Internal/SslRemOutput.h"
#include "../Util/StringHelp.h"
#include <openssl/err.h>
#include <stdio.h>
#include <errno.h>

namespace Celo {
namespace Events {

SslBufferedConnection::SslBufferedConnection( SSL_CTX *ssl_ctx, int fd ) : Event( fd ) {
    ssl = SSL_new( ssl_ctx );
    SSL_set_fd( ssl, fd );
    SSL_set_accept_state( ssl ); // handshake will be done by SSL_write and SSL_read
    SSL_set_mode( ssl, SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER );

    orig_to_write      = 0;
    stop_the_parsing   = false;
    already_polled_out = false;
    r_state            = want_nothing;
    w_state            = want_nothing;
}

SslBufferedConnection::SslBufferedConnection( VtableOnly vo ) : Event( vo ) {
}

SslBufferedConnection::~SslBufferedConnection() {
    if ( orig_to_write )
        free( orig_to_write );
    SSL_free( ssl );
}

void SslBufferedConnection::write_cst( const char *data, ST size, bool end ) {
    _write_ssl( data, size, end );
}

void SslBufferedConnection::write_cst( const char *data ) {
    return write_cst( data, strlen( data ) );
}

void SslBufferedConnection::write_str( const char *data, ST size, bool end ) {
    _write_ssl( data, size, end );
}

void SslBufferedConnection::write_str( const char *data ) {
    return write_str( data, strlen( data ) );
}

void SslBufferedConnection::write_fdd( int fd, ST off, ST len ) {
    TODO;
}

bool SslBufferedConnection::still_has_something_to_send() const {
    return orig_to_write != 0;
}

void SslBufferedConnection::_write_ssl( const char *data, ST size, bool end ) {
    // we want to write something but the preceding write has to be issued again and inp() / out() has not been called
    // -> store data to write in the buffer orig_to_write
    // (OK to change the args of SSL_write thanks to SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER)
    if ( w_state != want_nothing )
        return _append_to_data_to_write( data, size );

    // else, we can directly call SSL_write
    while ( size ) {
        ST ruff = SSL_write( ssl, data, size );

        switch ( SSL_get_error( ssl, ruff ) ) {
        case SSL_ERROR_NONE:
            data += ruff;
            size -= ruff;
            break;
        case SSL_ERROR_WANT_READ:
            w_state = want_to_read; // we will have to issue again the SSL_write() during the next inp()
            state.waiting_for_more_inp = true;
            return _append_to_data_to_write( data, size );
        case SSL_ERROR_WANT_WRITE:
            w_state = want_to_write; // we will have to issue again the SSL_write() during the next out()
            state.waiting_for_more_inp = true;
            if ( not already_polled_out ) {
                already_polled_out = true;
                poll_out();
            }
            return _append_to_data_to_write( data, size );
        case SSL_ERROR_SYSCALL:
        case SSL_ERROR_ZERO_RETURN:
            return reg_for_deletion();
        default:
            return ssl_err();
        }
    }
}

bool SslBufferedConnection::inp() {
    // need to issue again a SSL_write ?
    if ( w_state == want_to_read ) {
        while ( size_to_write ) {
            ST ruff = SSL_write( ssl, data_to_write, size_to_write );
            int	error = SSL_get_error( ssl, ruff );

            if ( error == SSL_ERROR_NONE ) {
                w_state = want_nothing;
                data_to_write += ruff;
                size_to_write -= ruff;
                if ( not size_to_write ) {
                    free( orig_to_write );
                    orig_to_write = 0;
                }
                continue;
            }

            if ( error == SSL_ERROR_WANT_READ ) {
                w_state = want_to_read; // we will have to issue again the SSL_write() during the next inp()
                break;
            }

            if ( error == SSL_ERROR_WANT_WRITE ) {
                w_state = want_to_write; // we will have to issue again the SSL_write() during the next out()
                if ( not already_polled_out ) {
                    already_polled_out = true;
                    poll_out();
                }
                break;
            }

            if ( error == SSL_ERROR_SYSCALL or error == SSL_ERROR_ZERO_RETURN )
                return true; // wait for the (RD)HUP event

            // else
            ssl_err();
            return false;
        }
    }

    // OK for a SSL_read ? (if want_to_write, the SSL_read has to be done during a call to out())
    if ( r_state != want_to_write ) {
        const int size_buff = 2048;
        char buff[ size_buff ];
        while ( true ) {
            ST ruff = SSL_read( ssl, buff, size_buff );
            int ecd = SSL_get_error( ssl, ruff );

            switch ( ecd ) {
            case SSL_ERROR_NONE:
                r_state = want_nothing;
                if ( stop_the_parsing or parse( buff, buff + ruff ) == false ) {
                    stop_the_parsing = true;
                    return still_has_something_to_send();
                }
                break;
            case SSL_ERROR_WANT_READ:
                r_state = want_to_read; // wait for the next inp()
                return true;
            case SSL_ERROR_WANT_WRITE:
                r_state = want_to_write; // wait for the next out()
                if ( not already_polled_out ) {
                    already_polled_out = true;
                    poll_out();
                }
                return true;
            case SSL_ERROR_SYSCALL:
            case SSL_ERROR_ZERO_RETURN:

                return true; // wait for the (RD)HUP event in the loop
            default:
                ssl_err();
                return false;
            }
        }
    }
}

bool SslBufferedConnection::out() {
    if ( r_state == want_to_write ) {
        const int size_buff = 2048;
        char buff[ size_buff ];
        while ( true ) {
            ST ruff = SSL_read( ssl, buff, size_buff );
            int error = SSL_get_error( ssl, ruff );

            if ( error == SSL_ERROR_NONE ) {
                r_state = want_nothing;
                if ( stop_the_parsing or parse( buff, buff + ruff ) == false ) {
                    stop_the_parsing = true;
                    if ( not still_has_something_to_send() )
                        return false;
                }
                continue;
            }

            if ( error == SSL_ERROR_WANT_READ ) {
                // wait for the next inp()
                r_state = want_to_read;
                break;
            }

            if ( error == SSL_ERROR_WANT_WRITE ) {
                // write to fd was not possible
                // -> we will have to issue again the SSL_read during a next call to out()
                r_state = want_to_write;
                break;
            }

            if ( error == SSL_ERROR_ZERO_RETURN or error == SSL_ERROR_SYSCALL ) // should fire a (RD)HUP event
                return true; // wait for the (RD)HUP event in the loop

            // else
            ssl_err();
            return false;
        }
    }


    if ( w_state == want_to_write ) {
        // try to issue again the SSL_write
        while ( true ) {
            // send to wbio
            ST ruff = SSL_write( ssl, data_to_write, size_to_write );
            int error = SSL_get_error( ssl, ruff );

            if ( error == SSL_ERROR_NONE ) {
                w_state = want_nothing;
                data_to_write += ruff;
                size_to_write -= ruff;
                if ( not size_to_write ) {
                    free( orig_to_write );
                    orig_to_write = 0;
                    break;
                }
                continue;
            }

            if ( error == SSL_ERROR_WANT_READ ) {
                // we will have to issue again the SSL_write() during the next inp()
                w_state = want_to_read;
                break;
            }

            if ( error == SSL_ERROR_WANT_WRITE ) {
                // we will have to issue again the SSL_write() during the next out()
                w_state = want_to_write;
                break;
            }

            if ( error == SSL_ERROR_ZERO_RETURN or error == SSL_ERROR_SYSCALL ) // should fire a (RD)HUP event
                return true; // wait for the (RD)HUP event in the loop

            //
            ssl_err();
            return false;
        }
    }

    return true;
}

void SslBufferedConnection::ssl_err() {
    ERR_print_errors_fp( stderr );
    errors |= EF_SSL_error;
    reg_for_deletion();
}

void SslBufferedConnection::_append_to_data_to_write( const char *data, ST size ) {
    if ( orig_to_write ) {
        char *n = (char *)malloc( size_to_write + size );
        memcpy( n, data_to_write, size_to_write );
        memcpy( n + size_to_write, data, size );
        size_to_write += size;
        free( orig_to_write );
        data_to_write = n;
        orig_to_write = n;
    } else {
        orig_to_write = (char *)malloc( size );
        memcpy( orig_to_write, data, size );
        data_to_write = orig_to_write;
        size_to_write = size;
    }
}

} // namespace Events
} // namespace Celo
