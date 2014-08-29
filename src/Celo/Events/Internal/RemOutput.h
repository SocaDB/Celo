#ifndef Celo_Events_Internal_RemOutput_H
#define Celo_Events_Internal_RemOutput_H

#include "../BufferedConnection.h"

#include <sys/sendfile.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

namespace Celo {
namespace Events {

/**
*/
struct RemOutput {
    typedef long long ST;
    enum { CLOSED = -2, ERROR = -1, WAIT = 0, DONE = 1 };
    virtual int write( BufferedConnection *obj ) = 0; ///< return CLOSED, ERROR, WAIT or DONE
    virtual ~RemOutput() {}
    RemOutput *next;
};

/**
*/
struct RemOutputData : public RemOutput {
    RemOutputData( const char *data, ST size, bool end ) : data( data ), size( size ), end( end ) {
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
    bool        end; ///< false to send using MSG_MORE
};

/**
*/
struct RemOutputCopiedData : public RemOutputData {
    RemOutputCopiedData( const char *data_to_be_copied, ST size, bool end ) : RemOutputData( 0, size, end ) {
        orig = (char *)malloc( size );
        memcpy( orig, data, size );
        data = orig;
    }

    virtual ~RemOutputCopiedData() {
        free( orig );
    }

    char *orig;
};

/**
*/
struct RemOutputFile : public RemOutput {
    RemOutputFile( int src, off_t off, ST size ) : src( src ), off( off ), size( size ) {
    }

    virtual ~RemOutputFile() {
        close( src );
    }

    virtual int write( BufferedConnection *obj ) {
        while ( true ) {
            ST real = sendfile( obj->fd, src, &off, size );
            if ( real <= 0 ) { // error ?
                if ( real == 0 )
                    return CLOSED;
                if ( errno == EAGAIN )
                    return WAIT;
                return ERROR;
            }

            size -= real;
            if ( not size ) // finished ?
                return DONE;
        }
    }

    int   src;
    off_t off;
    ST    size;
};


/**
*/
struct RemOutputBuffer : public RemOutput {
    RemOutputBuffer( Ptr<Buffer> buf, int off, bool end ) : buf( buf ), off( off ), end( end ) {
    }

    virtual int write( BufferedConnection *obj ) {
        while ( true ) {
            ST real = ::send( obj->fd, buf->data + off, buf->used - off, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
            if ( real <= 0 ) { // error ?
                if ( real == 0 )
                    return CLOSED;
                if ( errno == EAGAIN or errno == EWOULDBLOCK )
                    return WAIT;
                return ERROR;
            }

            off += real;
            if ( off >= buf->used ) {
                while ( true ) {
                    buf = buf->next;
                    if ( not buf )
                        return DONE;
                    if ( buf->used )
                        break;
                }
                off = 0;
            }
        }
    }

    Ptr<Buffer> buf;
    int         off; ///< offset in data->data
    bool        end; ///< false to send using MSG_MORE
};


/**
  return DONE if followed by something to send
*/
struct RemOutputWait : public RemOutput {
    virtual int write( BufferedConnection *obj ) {
        return next != 0;
    }
};

/**
  return WAIT such that it is appended RemOutput won't be sent
*/
struct RemOutputError : public RemOutput {
    virtual int write( BufferedConnection * ) {
        return WAIT;
    }
};

} // namespace Events
} // namespace Celo

#endif // Celo_Events_Internal_RemOutput_H
