#ifndef Celo_Events_BufferedConnection_H
#define Celo_Events_BufferedConnection_H

#include "Event.h"

namespace Celo {
namespace Events {

/**

*/
class BufferedConnection : public Event {
public:
    BufferedConnection( VtableOnly ); ///< a constructor that does not assign any attribute (else than the vtable). Permits to do a new( ptr ) T to change _only_ the vtable (underlying type)
    BufferedConnection( int fd );
    ~BufferedConnection();

    virtual void write_cst( const char *data, ST size, bool end = false ); ///< send "permanent" data (data won't be freed so we don't need to copy it if necessary to postpone the write)
    virtual void write_cst( const char *data ); ///< \0 ended version

    virtual void write_str( const char *data, ST size, bool end = false ); ///< send "non permanent" data, i.e. data will be copied if it cannot be sent immediatly
    virtual void write_str( const char *data ); ///< \0 ended version

    virtual void write_fdd( int fd, ST off, ST len ); ///< write data from file described by fd (its file descriptor)

    void wait_for_another_write(); ///< append RemOutputWait to the list of objects to be sent that will say "done" if followed by something to send

    bool still_has_something_to_send() const; ///< return true if write buffer is not empty

protected:
    virtual void inp();
    virtual void out();

    // to be redefined
    virtual bool parse( char *beg, char *end ) = 0; ///< return false if we have enough data

    template<class T> void _write( const char *data, ST size, bool end );
    template<class T> void _write( int fd, ST off, ST size, bool end );
    void cl_rem_and_add_err(); ///< call cl_rem and append( RemOutputError )
    void append( struct RemOutput *rem_out );
    void cl_rem(); ///< delete list of rem_out
    bool flush(); ///< return true if effectively flushed

    // attributes
    struct RemOutput *prim_rem_out;
    struct RemOutput *last_rem_out;
};

} // namespace Events
} // namespace Celo

#endif // Celo_Events_BufferedConnection_H
