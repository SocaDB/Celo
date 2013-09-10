#ifndef SSLLISTENER_H
#define SSLLISTENER_H

#include "Listener.h"

/**
*/
class SslListener : public Listener {
public:
    SslListener( const char *port, const char *cert_file, const char *key_file );
    SslListener( VtableOnly );
    virtual bool inp();
};

#endif // SSLLISTENER_H
