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


#ifndef STRINGCPN_H
#define STRINGCPN_H

#include <algorithm>
#include <stdlib.h>
#include <string.h>

/**
*/
struct StringCpn {
    StringCpn() : rese( 0 ) {
    }

    ~StringCpn() {
        if ( rese )
            free( data );
    }

    /// make an own copy if it's not the case (rese=0 means that data is not owned by this)
    void make_own_copy_if_not_the_case() {
        if ( rese )
            return;
        const char *old = data;
        rese = std::max( 1024, 2 * size );
        data = (char *)malloc( rese );
        memcpy( data, old, size );
    }

    /// ensure room for at least 1 char
    void make_room_for_at_least_1_char() {
        if ( size == rese ) {
            char *old = data;
            data = (char *)malloc( rese *= 2 );
            memcpy( data, old, size );
            free( old );
        }
    }


    char *data;
    int   rese;
    int   size;
};


#endif // STRINGCPN_H
