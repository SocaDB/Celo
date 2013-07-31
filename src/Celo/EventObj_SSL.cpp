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


#include "EventObj_SSL.h"
#include <unistd.h>
#include <errno.h>

#include "StringHelp.h"

EventObj_SSL::EventObj_SSL( int fd ) : EventObj( fd ) {
}

EventObj_SSL::EventObj_SSL( VtableOnly vo ) : EventObj( vo ) {
}

bool EventObj_SSL::inp() {
    const int size_buff = 2048;
    char buff[ size_buff ];
    while ( true ) {
        ST ruff = read( fd, buff, size_buff );
        if ( ruff < 0 ) {
            // EAGAIN
            if ( errno == EAGAIN )
                return true;
            return false;
        }

        // we need more data but we don't have ?
        if ( ruff == 0 )
            return true;

        // parse
        // write( 0, buff, ruff );
        int p = parse( buff, buff + ruff );

        if ( p )
            return p < 0;
    }
}

bool EventObj_SSL::cnt_default_value() const {
    return false;
}


