#include "Buffer.h"
#include <string.h>

namespace Celo {

Buffer::Buffer() : used( 0 ) {
}

void Buffer::write_to_stream( Stream &os ) const {
    for( int i = 0; i < size(); ++i ) {
        static const char *c = "0123456789abcdef";
        os << ( i ? " " : "" ) << c[ (*this)[ i ] / 16 ] << c[ (*this)[ i ] % 16 ];
    }
    // os << "\n";
}

const PI8 &Buffer::operator[]( ST off ) const {
    for( const Buffer *b = this; ; b = b->next.ptr() ) {
        if ( off < b->used )
            return b->data[ off ];
        off -= b->used;
    }
}

PI8 &Buffer::operator[]( ST off ) {
    for( Buffer *b = this; ; b = b->next.ptr() ) {
        if ( off < b->used )
            return b->data[ off ];
        off -= b->used;
    }
}

const PI8 *Buffer::ptr( ST off ) const {
    return &operator[]( off );
}

PI8 *Buffer::ptr( ST off ) {
    return &operator[]( off );
}

ST Buffer::size() const {
    ST res = 0;
    for( const Buffer *b = this; b; b = b->next.ptr() )
        res += b->used;
    return res;
}

bool Buffer::empty() const {
    for( const Buffer *b = this; b; b = b->next.ptr() )
        if ( b->used )
            return true;
    return false;
}

ST Buffer::item_room() const {
    return item_size - used;
}

void Buffer::cut( int off ) {
    Ptr<Buffer> t = new Buffer;

    t->used = used - off;
    used = off;

    memcpy( t->data, data + off, t->used );

    t->next = next;
    next = t;
}

void Buffer::copy_to( PI8 *res ) {
    for( const Buffer *b = this; b; b = b->next.ptr() ) {
        memcpy( res, b->data, b->used );
        res += b->used;
    }
}


}
