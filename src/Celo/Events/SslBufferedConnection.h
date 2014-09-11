#ifndef SSLBUFFEREDCONNECTION_H
#define SSLBUFFEREDCONNECTION_H

#include "../System/Buffer.h"
#include <openssl/ssl.h>
#include <stdlib.h> // for off_t
#include "Event.h"

namespace Celo {
namespace Events {

/**
  SSL_CTX can be constructed using SslCtx e.g.
    ssl_ctx( "mycert.pem", "mycert.pem" );
*/
class SslBufferedConnection : public Event {
public:
    typedef long long ST;

    SslBufferedConnection( SSL_CTX *ssl_ctx, int fd, bool server = true );
    SslBufferedConnection( VtableOnly );
    ~SslBufferedConnection();

    virtual void write_cst( const char *data, ST size, bool end = false );
    virtual void write_cst( const char *data );

    virtual void write_str( const char *data, ST size, bool end = false );
    virtual void write_str( const char *data );

    virtual void write_fdd( int fd, ST off, ST len );

    virtual void write_buf( Ptr<Buffer> buf, int off = 0, bool end = false ); ///< write data from buf. BEWARE, buf will be set to 0 (data is owned by this)

protected:
    virtual void inp();
    virtual void out();
    virtual void ssl_error();

    // to be redefined
    virtual bool parse( Ptr<Buffer> buffer ) = 0; ///< return false if we have enough data

private:
    typedef enum {
        want_nothing  = 0,
        want_to_read  = 1,
        want_to_write = 2
    } State;

    enum {
        EF_SSL_error = _last_Event_EF * 2
    };

    void _write_ssl( const char *data, ST size, bool end );
    void _append_to_data_to_write( const char *data, ST size );
    bool _still_has_something_to_send() const; ///< return true if write buffer is not empty

    SSL  *ssl;
    State r_state;
    State w_state;
    char *orig_to_write;
    char *data_to_write;
    ST    size_to_write;
    bool  stop_the_parsing;
    bool  already_polled_out;
};

} // namespace Events
} // namespace Celo

#endif // SSLBUFFEREDCONNECTION_H
