#include "FilteredAndBufferedConnection.h"
#include "../Util/StringHelp.h"
#include "../Filters/Filter.h"

#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

namespace Celo {
namespace Events {

namespace Internal {

struct NewInpFunctor : public Filter::OutputFunctor {
    virtual bool operator()( const char *data, ST size, bool end ) {
        return f->parse( data, data + size );
    }
    FilteredAndBufferedConnection *f;
};

struct NewOutFunctor : public Filter::OutputFunctor {
    virtual bool operator()( const char *data, ST size, bool end ) {
        f->Writable::write_str( data, size, end );
        return true;
    }
    FilteredAndBufferedConnection *f;
};

} // namespace Internal

FilteredAndBufferedConnection::FilteredAndBufferedConnection( int fd, Filter *out_filter, Filter *inp_filter ) : Writable( fd ), out_filter( out_filter ), inp_filter( inp_filter ) {
}

FilteredAndBufferedConnection::FilteredAndBufferedConnection( VtableOnly vo ) : Writable( vo ) {
}


void FilteredAndBufferedConnection::write_cst( const char *data, ST size, bool end ) {
    return _write_raw( data, size, end );
}

void FilteredAndBufferedConnection::write_cst( const char *data ) {
    return write_str( data, strlen( data ), false );
}

void FilteredAndBufferedConnection::write_str( const char *data, ST size, bool end ) {
    return _write_raw( data, size, end );
}

void FilteredAndBufferedConnection::write_str( const char *data ) {
    return write_str( data, strlen( data ), false );
}

void FilteredAndBufferedConnection::write_fdd( int fd, ST off, ST len ) {
    TODO;
}

bool FilteredAndBufferedConnection::inp() {
    Internal::NewInpFunctor new_inp_functor;
    new_inp_functor.f = this;

    // send data to filter
    while ( true ) {
        const int size_buff = 2048;
        char buff[ size_buff ];
        ST ruff = read( fd, buff, size_buff );
        if ( ruff <= 0 ) {
            // end of available data (but not end of the connection)
            if ( ruff < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                break;
            err();
            return false;
        }

        if ( not inp_filter->operator()( new_inp_functor, buff, ruff ) )
            return false;
    }

    // say that all the data has been retrieved
    return inp_filter->update( new_inp_functor );
}

void FilteredAndBufferedConnection::_write_raw( const char *data, ST size, bool end ) {
    Internal::NewOutFunctor new_out_functor;
    new_out_functor.f = this;

    out_filter->operator()( new_out_functor, data, size, end );
}


} // namespace Events
} // namespace Celo

