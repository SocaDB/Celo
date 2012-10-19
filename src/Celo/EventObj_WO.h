/*
 Copyright 2012 Structure Computation  www.structure-computation.com
 Copyright 2012 Hugo Leclerc

 This file is part of Celo.

 Celo is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Celo is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with Celo. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef EVENTOBJ_WO_H
#define EVENTOBJ_WO_H

#include "EventObj_WP.h"

/**
  EventObj with helper methods for input / output of data
*/
class EventObj_WO : public EventObj_WP {
public:
    EventObj_WO( VtableOnly );
    EventObj_WO( int fd );
    virtual ~EventObj_WO();

    // send data to fd
    void send_cst( const char *data, ST size, bool end = true ); ///< permanent data (no copy, no free)
    void send_cst( const char *data ); ///< permanent data (no copy, no free)

    void send_str( const char *data, ST size, bool end = true ); ///< data will be copied (if not sent in one shot) and freed
    void send_str( const char *data ); ///< data will be copied (if not sent in one shot) and freed

    void send_fid( int src, ST off, ST len ); ///< from file id src

    void wait_for_send(); ///< wait until there is something to send (that is a send_... is issued)

    bool still_has_something_to_send() const;

protected:
    virtual bool out();

private:
    template<class T> friend void _send( EventObj_WO *eo, const char *data, ST size, bool end );
    void append( struct RemOutput *rem_out );
    void cl_rem();

    struct RemOutput *prim_rem_out;
    struct RemOutput *last_rem_out;
};

#endif // EVENTOBJ_WO_H
