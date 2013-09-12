#ifndef FILTER_SSL_H
#define FILTER_SSL_H

#include <openssl/ssl.h>
#include "Filter.h"

namespace Celo {

/**
*/
class Filter_SSL : public Filter {
public:
    Filter_SSL( SSL_CTX *ctx );
    ~Filter_SSL();

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

}

#endif // FILTER_SSL_H
