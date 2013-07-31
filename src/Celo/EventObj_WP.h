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


#ifndef EVENTOBJ_WP_H
#define EVENTOBJ_WP_H

#include "EventObj.h"

/**
  EventObj With parse
*/
class EventObj_WP : public EventObj {
public:
    enum { WAIT = -1, CNT = 0, OK = 1, KO = 2 };
    EventObj_WP( VtableOnly );
    EventObj_WP( int fd );
    virtual bool inp();

protected:
    // to be redefined (if default inp is used)
    virtual int parse( char *beg, char *end ) = 0; ///< return CNT if we need more to read. WAIT permits to keep the connection alive

    virtual bool cnt_default_value() const;
};

#endif // EVENTOBJ_WP_H
