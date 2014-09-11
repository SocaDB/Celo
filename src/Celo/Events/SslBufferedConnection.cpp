#include "SslBufferedConnection.h"
#include <openssl/err.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>



namespace Celo {
namespace Events {

void ShowCerts( SSL* ssl ) {
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate( ssl ); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("No certificates.\n");
}

SslBufferedConnection::SslBufferedConnection( SSL_CTX *ssl_ctx, int fd, bool server ) : Event( fd ) {
    if ( ssl_ctx ) {
        ssl = SSL_new( ssl_ctx );
        SSL_set_fd( ssl, fd );
        if ( server )
            SSL_set_accept_state ( ssl ); // handshake will be done by SSL_write and SSL_read
        else
            SSL_set_connect_state( ssl ); // handshake will be done by SSL_write and SSL_read

        SSL_set_mode( ssl, SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER );
    } else
        ssl = 0;

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
    if ( ssl )
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
    printf( "TODO: SslBufferedConnection::write_fdd\n" );
}

void SslBufferedConnection::write_buf( Ptr<Buffer> buf, int off, bool end ) {
    // loop to remove `off` bytes
    for( Buffer *b = buf.ptr(); b; off -= b->used, b = b->next.ptr() ) {
        // we have someting to write ?
        if ( off < b->used ) {
            _write_ssl( (const char *)b->data + off, b->used - off, false );
            while( ( b = b->next.ptr() ) )
                _write_ssl( (const char *)b->data, b->used, false );
            return;
        }
    }
}

bool SslBufferedConnection::_still_has_something_to_send() const {
    return orig_to_write != 0;
}

void SslBufferedConnection::_write_ssl( const char *data, ST size, bool end ) {
    // we want to write something but the preceding write has to be issued again during an inp() or out()
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
            _append_to_data_to_write( data, size );
            w_state = want_to_read; // we will have to issue again the SSL_write() during the next inp()
            return wait_for_more_inp();
        case SSL_ERROR_WANT_WRITE:
            _append_to_data_to_write( data, size );
            w_state = want_to_write; // we will have to issue again the SSL_write() during the next out()
            return wait_for_more_out();
        case SSL_ERROR_ZERO_RETURN:
            return rd_hup_error();
        case SSL_ERROR_SYSCALL:
            return hup_error();
        default:
            return ssl_error();
        }
    }
}

void SslBufferedConnection::inp() {
    // not an ssl connection ?
    if ( not ssl ) {
        while ( true ) {
            Ptr<Buffer> buff = new Buffer;
            buff->used = ::read( fd, buff->data,  buff->item_size );
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
        return;
    }


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
                return wait_for_more_inp(); // not useful to try a SSL_read (so we can return immediatly)
            }

            if ( error == SSL_ERROR_WANT_WRITE ) {
                w_state = want_to_write; // we will have to issue again the SSL_write() during the next out()
                wait_for_more_out();
                break;
            }

            if ( error == SSL_ERROR_ZERO_RETURN )
                return rd_hup_error();

            if ( error == SSL_ERROR_SYSCALL )
                return hup_error();

            // else
            return ssl_error();
        }
    }

    // OK for a SSL_read ? (if want_to_write, the SSL_read has to be done during a call to out())
    if ( r_state != want_to_write ) {
        Ptr<Buffer> buff = new Buffer;
        while ( true ) {
            // update or make a new buffer to store the incoming data
            if ( buff->cpt_use > 1 )
                buff = new Buffer;

            buff->used = SSL_read( ssl, buff->data, buff->item_size );
            int ecd = SSL_get_error( ssl, buff->used );

            switch ( ecd ) {
            case SSL_ERROR_NONE:
                r_state = want_nothing;
                if ( stop_the_parsing or parse( buff ) == false ) {
                    stop_the_parsing = true;
                    return;
                }
                break;
            case SSL_ERROR_WANT_READ:
                r_state = want_to_read; // wait for the next inp()
                return wait_for_more_inp();
            case SSL_ERROR_WANT_WRITE:
                r_state = want_to_write; // wait for the next out()
                return wait_for_more_out();
            case SSL_ERROR_ZERO_RETURN:
                return rd_hup_error();
            case SSL_ERROR_SYSCALL:
                return hup_error();
            default:
                return ssl_error();
            }
        }
    }
}

void SslBufferedConnection::out() {
    if ( not ssl ) {
        while ( true ) {
            ST real = ::send( fd, data_to_write, size_to_write, MSG_NOSIGNAL );
            if ( real <= 0 ) {
                if ( real == 0 )
                    return reg_for_deletion(); // CLOSED !
                if ( errno == EAGAIN or errno == EWOULDBLOCK )
                    return; // PLEASE RETRY LATER
                return reg_for_deletion(); // ERROR !
            }

            size_to_write -= real;
            if ( not size_to_write ) {
                free( orig_to_write );
                orig_to_write = 0;
                return; // DONE
            }
            data_to_write += real;
        }
    }

    if ( r_state == want_to_write ) {
        Ptr<Buffer> buff = new Buffer;
        while ( true ) {
            // update or make a new buffer to store the incoming data
            if ( buff->cpt_use > 1 )
                buff = new Buffer;

            buff->used = SSL_read( ssl, buff->data, buff->item_size );
            int error = SSL_get_error( ssl, buff->used );

            if ( error == SSL_ERROR_NONE ) {
                r_state = want_nothing;
                if ( stop_the_parsing or parse( buff ) == false ) {
                    stop_the_parsing = true;
                    break;
                }
                continue;
            }

            if ( error == SSL_ERROR_WANT_READ ) {
                r_state = want_to_read;
                wait_for_more_inp();
                break;
            }

            if ( error == SSL_ERROR_WANT_WRITE ) {
                // write to fd was not possible
                // -> we will have to issue again the SSL_read during a next call to out()
                r_state = want_to_write;
                wait_for_more_out( false );
                break;
            }

            if ( error == SSL_ERROR_ZERO_RETURN ) // should fire a (RD)HUP event
                return rd_hup_error();

            if ( error == SSL_ERROR_SYSCALL ) // should fire a (RD)HUP event
                return hup_error();

            // else
            return ssl_error();
        }
    }


    if ( w_state == want_to_write or ( w_state == want_nothing and _still_has_something_to_send() ) ) {
        // try to issue again the SSL_write
        while ( true ) {
            // send to wbio
            ST ruff = SSL_write( ssl, data_to_write, size_to_write );

            switch ( SSL_get_error( ssl, ruff ) ) {
            case SSL_ERROR_NONE:
                w_state = want_nothing;
                data_to_write += ruff;
                size_to_write -= ruff;
                if ( not size_to_write ) {
                    free( orig_to_write );
                    orig_to_write = 0;
                    return;
                }
                break;
            case SSL_ERROR_WANT_READ:
                w_state = want_to_read;
                return wait_for_more_inp();
            case SSL_ERROR_WANT_WRITE:
                w_state = want_to_write;
                return wait_for_more_out();
            case SSL_ERROR_ZERO_RETURN:
                return rd_hup_error();
            case SSL_ERROR_SYSCALL:
                return hup_error();
            default:
                return ssl_error();
            }
        }
    }
}

void SslBufferedConnection::ssl_error() {
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
