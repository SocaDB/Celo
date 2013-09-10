#ifndef SSLCTX_H
#define SSLCTX_H

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/bio.h"

/**
*/
class SslCtx {
public:
    SslCtx( const char *cert_file, const char *key_file );

    SSL_CTX *ctx;
};

#endif // SSLCTX_H
