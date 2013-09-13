#ifndef CELO_FILTERED_H
#define CELO_FILTERED_H

#include "Writable.h"

namespace Celo {
namespace Internal {
struct NewInpFunctor;
struct NewOutFunctor;
}
class Filter;

/**
  Event_obj which calls parse( beg, end ) each time new data comes in
*/
class Filtered : public Writable {
public:
    Filtered( int fd, Filter *out_filter, Filter *inp_filter );
    Filtered( VtableOnly vo );

    virtual void write_cst( const char *data, ST size, bool end = false ); ///< send "permanent" data (data won't be freed so we don't need to copy it if necessary to postpone the write)
    virtual void write_cst( const char *data ); ///< \0 ended version
    virtual void write_str( const char *data, ST size, bool end = false ); ///< send "non permanent" data, i.e. data will be copied if it cannot be sent immediatly
    virtual void write_str( const char *data ); ///< \0 ended version
    virtual void write_fdd( int fd, ST off, ST len ); ///< write data from file described by fd (its file descriptor)

protected:
    virtual bool inp();

    // to be redefined
    virtual bool parse( const char *beg, const char *end ) = 0; ///< return false if we have enough data

    Filter *inp_filter;
    Filter *out_filter;

private:
    void _write_raw( const char *data, ST size, bool end ); ///< send to filter

    struct RemOutput *prim_rem_out;
    struct RemOutput *last_rem_out;

    friend struct Internal::NewInpFunctor;
    friend struct Internal::NewOutFunctor;
};

}

#endif // CELO_FILTERED_H
