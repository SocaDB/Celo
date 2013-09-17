#ifndef Celo_Events_Internal_SSLREMOUTPUT_H
#define Celo_Events_Internal_SSLREMOUTPUT_H

#include "RemOutput.h"

namespace Celo {
namespace Events {

/**
*/
struct SslRemOutput : public RemOutput {
    SslRemOutput( const char *data, ST size, bool end ) : data( data ), size( size ), end( end ) {
        orig = (char *)malloc( size );
        memcpy( orig, data, size );
        data = orig;
    }

    virtual ~SslRemOutput() {
        free( orig );
    }

    virtual int write( BufferedConnection *obj ) {
        while ( true ) {
            ST real = ::send( obj->fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
            if ( real <= 0 ) { // error ?
                if ( real == 0 )
                    return CLOSED;
                if ( errno == EAGAIN or errno == EWOULDBLOCK )
                    return WAIT;
                return ERROR;
            }

            size -= real;
            if ( not size ) // finished ?
                return DONE;
            data += real;
        }
    }

    const char *data;
    ST          size;
    char       *orig;
    bool        end; ///< false to send using MSG_MORE
};

} // namespace Events
} // namespace Celo


#endif // Celo_Events_Internal_SSLREMOUTPUT_H
