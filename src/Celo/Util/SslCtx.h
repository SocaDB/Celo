#ifndef SSLCTX_H
#define SSLCTX_H

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/bio.h"

namespace Celo {

/**
*/
class SslCtx {
public:
    SslCtx( const char *cert_file, const char *key_file ); ///< for a server connection
    SslCtx(); ///< for a client connection

    SSL_CTX *ctx;
};

} // namespace Celo

#endif // SSLCTX_H
