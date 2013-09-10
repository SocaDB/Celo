#ifndef EVENTOBJ_WO_SSL_H
#define EVENTOBJ_WO_SSL_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "EventObj.h"

/**
*/
class EventObj_WO_SSL : public EventObj {
public:
    enum { WAIT = -1, CNT = 0, OK = 1, KO = 2 };
    EventObj_WO_SSL( SSL_CTX *ctx, int fd );
    EventObj_WO_SSL( VtableOnly );
    virtual bool inp();
    virtual bool out();

    void send_cst( const char *data, ST size, bool end = true ); ///< send permanent data (no copy, no free)
    void send_cst( const char *data ); ///< send permanent data (no copy, no free)

    void send_str( const char *data, ST size, bool end = true ); ///< send data will be copied (if not sent in one shot) and freed
    void send_str( const char *data ); ///< send data will be copied (if not sent in one shot) and freed

    void send_fid( int src, ST off, ST len ); ///< send data from a file describer by its id

protected:
    /// to be redefined
    ///
    /// return CNT if we need more to read. WAIT permits to keep the connection alive (e.g. for an output to be computed)
    virtual int parse( char *beg, char *end ) = 0;

    virtual bool cnt_default_value() const;

private:
    bool _continue_ssl( int function_return = 0 );

    SSL_CTX *ctx;
    SSL     *ssl;
    BIO     *rbio;
    BIO     *wbio;
};

#endif // EVENTOBJ_WO_SSL_H
