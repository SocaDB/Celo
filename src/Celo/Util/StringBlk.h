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


#ifndef STRINGBLK_H
#define STRINGBLK_H

#include "../TypeConfig.h"
#include "StringHelp.h"

namespace Celo {

/**
*/
class StringBlk {
public:
    struct Item {
        Item *next( ST &tot ) { tot -= loc; return reinterpret_cast<Item *>( dat + loc ); }
        const Item *next( ST &tot ) const { tot -= loc; return reinterpret_cast<const Item *>( dat + loc ); }

        char *dat;
        ST    loc;
    };

    StringBlk();

    ST size() const { return _tot; }

    Item _beg;
    ST   _tot;
};

inline std::ostream &operator<<( std::ostream &os, const StringBlk &s ) {
    ST tot = s._tot;
    for( const StringBlk::Item *item = &s._beg; tot; item = item->next( tot ) )
        os.write( item->dat, item->loc );
    return os;
}

}

#endif // STRINGBLK_H
