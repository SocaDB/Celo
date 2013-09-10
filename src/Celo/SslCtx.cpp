#include "SslCtx.h"

#pragma lib_name crypto
#pragma lib_name ssl

static bool ssl_initialized = false;

void init_ssl_if_necessary() {
    if ( ssl_initialized )
        return;
    ssl_initialized = true;

    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms(); // load & register all cryptos, etc.
    SSL_library_init();
}

SslCtx::SslCtx( const char *cert_file, const char *key_file ) {
    init_ssl_if_necessary();

    // server ctx
    const SSL_METHOD *method = SSLv23_server_method(); // create new server-method instance
    ctx = SSL_CTX_new( method ); // create new context from method
    if ( not ctx ) {
        ERR_print_errors_fp( stderr );
        abort();
    }

    // set the local certificate from cert_file
    if ( SSL_CTX_use_certificate_file( ctx, cert_file, SSL_FILETYPE_PEM ) <= 0 ) {
        ERR_print_errors_fp( stderr );
        abort();
    }
    // set the private key from key_file (may be the same as CertFile)
    if ( SSL_CTX_use_PrivateKey_file( ctx, key_file, SSL_FILETYPE_PEM ) <= 0 ) {
        ERR_print_errors_fp( stderr );
        abort();
    }
    // verify private key
    if ( not SSL_CTX_check_private_key( ctx ) ) {
        fprintf( stderr, "Private key does not match the public certificate\n" );
        abort();
    }
}
