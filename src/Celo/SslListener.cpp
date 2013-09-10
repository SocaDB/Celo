#include "openssl/ssl.h"
#include "openssl/err.h"
#include "SslListener.h"
#include "SocketHelp.h"
#include "EventLoop.h"
#include "SslHelp.h"

SslListener::SslListener( const char *port, const char *cert_file, const char *key_file ) : Listener( port ) {
    init_ssl_if_necessary();

    // server ctx
    SSL_METHOD *method = SSLv2_server_method(); // create new server-method instance
    SSL_CTX    *ctx    = SSL_CTX_new( method ); // create new context from method
    if ( not ctx ) {
        ERR_print_errors_fp( stderr );
        abort();
    }

    // set the local certificate from cert_file
    if ( SSL_CTX_use_certificate_file( ctx, cert_file, SSL_FILETYPE_PEM ) <= 0 ) {
        ERR_print_errors_fp(stderr);
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

SslListener::SslListener( VtableOnly vt ) : Listener( vt ) {

}

bool SslListener::inp() {
    while ( true ) {
        int nd = accepting_socket( fd );
        if ( nd == -1 )
            return true; // continue to listen

        SSL *ssl;

        *ev_loop << event_obj_factory( nd );
    }
}
