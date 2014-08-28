#ifndef Celo_Events_FilteredAndBufferedConnection_H
#define Celo_Events_FilteredAndBufferedConnection_H

#include "BufferedConnection.h"

namespace Celo {
namespace Filters {
class Filter;
}

namespace Events {

// declaration ext objects
namespace Internal {
struct NewInpFunctor;
struct NewOutFunctor;
}

/**
  Event_obj which calls parse( beg, end ) each time new data comes in
*/
class FilteredAndBufferedConnection : public BufferedConnection {
public:
    FilteredAndBufferedConnection( int fd, Filter *out_filter, Filter *inp_filter );
    FilteredAndBufferedConnection( VtableOnly vo );

    virtual void write_cst( const char *data, ST size, bool end = false ); ///< send "permanent" data (data won't be freed so we don't need to copy it if necessary to postpone the write)
    virtual void write_cst( const char *data ); ///< \0 ended version
    virtual void write_str( const char *data, ST size, bool end = false ); ///< send "non permanent" data, i.e. data will be copied if it cannot be sent immediatly
    virtual void write_str( const char *data ); ///< \0 ended version
    virtual void write_fdd( int fd, ST off, ST len ); ///< write data from file described by fd (its file descriptor)

protected:
    virtual bool inp();

    Filter *inp_filter;
    Filter *out_filter;

private:
    void _write_raw( const char *data, ST size, bool end ); ///< send to filter

    struct RemOutput *prim_rem_out;
    struct RemOutput *last_rem_out;

    friend struct Internal::NewInpFunctor;
    friend struct Internal::NewOutFunctor;
};

} // namespace Events
} // namespace Celo

#endif // Celo_Events_FilteredAndBufferedConnection_H
