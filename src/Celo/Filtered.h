#ifndef CELO_FILTERED_H
#define CELO_FILTERED_H

#include "EventObj.h"

namespace Celo {
class Filter;

/**
  Event_obj which calls parse( beg, end ) each time new data comes in
*/
class Filtered : public EventObj {
public:
    Filtered( VtableOnly vo );
    Filtered( int fd, Filter *filter );

    void write_cst( const char *data, ST size, bool end = false ); ///< send "permanent" data (data won't be freed so we don't need to copy it if necessary to postpone the write)
    void write_cst( const char *data ); ///< \0 ended version

    void write_str( const char *data, ST size, bool end = false ); ///< send "non permanent" data, i.e. data will be copied if it cannot be sent immediatly
    void write_str( const char *data ); ///< \0 ended version

    void write_fdd( int fd, ST off, ST len ); ///< write data from file described by fd (its file descriptor)

    void wait_for_another_write(); ///< append RemOutputWait to the list of objects to be sent that will say "done" if followed by something to send

protected:
    virtual bool inp();
    virtual bool out();

    // to be redefined
    virtual bool parse( char *beg, char *end ) = 0; ///< return false if we have enough data

    Filter *filter;

private:
    void _send_unfiltered( const char *data, ST size, bool end );
};

}

#endif // CELO_FILTERED_H
