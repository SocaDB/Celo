#include "Util/StringHelp.h"
#include <openssl/err.h>
#include "Filter_SSL.h"

namespace Celo {

Filter_SSL::Filter_SSL( SSL_CTX *ctx ) {
    rbio = BIO_new( BIO_s_mem() );
    wbio = BIO_new( BIO_s_mem() );

    ssl = SSL_new( ctx );

    SSL_set_mode( ssl, SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER );

    SSL_set_accept_state( ssl );
    SSL_set_bio( ssl, rbio, wbio );
}

Filter_SSL::~Filter_SSL() {
    SSL_free( ssl );
}

ST Filter_SSL::new_inp( const char *data, ST size ) {
    return BIO_write( rbio, data, size );
}

ST Filter_SSL::new_out( const char *data, ST size ) {
    // try write encoded data to wbio
    ST sent = SSL_write( ssl, data, size );

    // everything OK ?
    switch ( SSL_get_error( ssl, sent ) ) {
    case SSL_ERROR_NONE:
        return sent;
    case SSL_ERROR_ZERO_RETURN:
        // end of the connection
        return END_OF_THE_CONNECTION;
    case SSL_ERROR_WANT_READ:
        return NEED_MORE_INPUT;
    case SSL_ERROR_WANT_WRITE:
        TODO; // should not happen (we're using BIO) !
        return ERROR;
    default:
        return ERROR;
    }
}

ST Filter_SSL::get_inp( char *data, ST max_size ) {
    // try to read data
    ST read = SSL_read( ssl, data, max_size );

    // OK ?
    switch ( SSL_get_error( ssl, read ) ) {
    case SSL_ERROR_NONE:
        return read;
    case SSL_ERROR_ZERO_RETURN:
        return END_OF_THE_CONNECTION;
    case SSL_ERROR_WANT_READ:
        return NEED_MORE_INPUT;
    case SSL_ERROR_WANT_WRITE:
        TODO; // should not happen (we're using BIO) !
        return ERROR;
    default:
        return ERROR;
    }
}

ST Filter_SSL::get_out( char *data, ST max_size ) {
    return BIO_read( wbio, data, max_size );
}

}
