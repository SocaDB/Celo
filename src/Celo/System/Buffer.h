#ifndef Celo_BUFFER_H
#define Celo_BUFFER_H

#include <algorithm>
#include <stdint.h>
#include "Ptr.h"

namespace Celo {

/**
*/
class Buffer : public ObjectWithCptUse {
public:
    typedef unsigned char UC;
    typedef int64_t       SL;

    enum { item_size = 1024 - sizeof( Ptr<Buffer> ) - sizeof( int ) };

    Buffer();

    template<class Stream>
    void write_to_stream( Stream &os, SL off = 0, SL len = - 1 ) const {
        for( SL i = off, e = len < 0 ? size() : std::min( off + len, size() ); i < e; ++i ) {
            static const char *c = "0123456789abcdef";
            os << ( i > off ? " " : "" ) << c[ (*this)[ i ] / 16 ] << c[ (*this)[ i ] % 16 ];
        }
    }

    template<class Stream>
    void write_char( Stream &os, SL off = 0, SL len = - 1 ) const {
        for( SL i = off, e = len < 0 ? size() : std::min( off + len, size() ); i < e; ++i )
            os << (*this)[ i ];
    }

    const UC &operator[]( SL off ) const;
    UC &operator[]( SL off );

    const UC *ptr( SL off ) const;
    UC *ptr( SL off );

    SL size() const; ///< total size, starting from this and using next
    bool empty() const;

    SL item_room() const; ///< available room in this (`next` excluded)

    void cut( SL off ); ///< split this into two buffer (store the second in next). Mainly for test purpose

    void copy_to( UC *res );

    static bool skip_some( Ptr<Buffer> &buf, SL &off, SL &end, SL msg ); ///< update buffer and off_buffer to skip msg_length
    static bool skip_some( Ptr<Buffer> &buf, SL &off, SL len ); ///< update buffer and off_buffer to skip msg_length

    // attributes
    Ptr<Buffer> next;
    int         used;
    UC          data[ item_size ];
};

}

#endif // Celo_BUFFER_H
