#include "Buffer.h"
#include <string.h>

namespace Celo {

Buffer::Buffer() : used( 0 ) {
}

const Buffer::PI8 &Buffer::operator[]( SI64 off ) const {
    for( const Buffer *b = this; ; b = b->next.ptr() ) {
        if ( off < b->used )
            return b->data[ off ];
        off -= b->used;
    }
}

Buffer::PI8 &Buffer::operator[]( SI64 off ) {
    for( Buffer *b = this; ; b = b->next.ptr() ) {
        if ( off < b->used )
            return b->data[ off ];
        off -= b->used;
    }
}

const Buffer::PI8 *Buffer::ptr( SI64 off ) const {
    return &operator[]( off );
}

Buffer::PI8 *Buffer::ptr( SI64 off ) {
    return &operator[]( off );
}

Buffer::SI64 Buffer::size() const {
    SI64 res = 0;
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

Buffer::SI64 Buffer::item_room() const {
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
