#ifndef STRINGBLK_H
#define STRINGBLK_H

#include "StringHelp.h"
#include "Config.h"

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

#endif // STRINGBLK_H
