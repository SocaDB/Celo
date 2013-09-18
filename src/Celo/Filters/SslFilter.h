#ifndef Celo_Filters_SslFilter_H
#define Celo_Filters_SslFilter_H

#include <openssl/ssl.h>
#include "Filter.h"

namespace Celo {
namespace Filters {

/**
*/
class SslFilter : public Filter {
public:
    SslFilter( SSL_CTX *ctx );
    ~SslFilter();

    virtual ST new_inp( const char *data, ST size );
    virtual ST new_out( const char *data, ST size );

    virtual ST get_inp( char *data, ST max_size );
    virtual ST get_out( char *data, ST max_size );

protected:
    SSL_CTX *ctx;
    SSL     *ssl;
    BIO     *rbio;
    BIO     *wbio;
};

} // namespace Celo
} // namespace Filters

#endif // Celo_Filters_SslFilter_H
