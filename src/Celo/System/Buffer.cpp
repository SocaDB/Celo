#include "Buffer.h"
#include <string.h>

namespace Celo {

Buffer::Buffer() : used( 0 ) {
}

const Buffer::UC &Buffer::operator[]( SL off ) const {
    for( const Buffer *b = this; ; b = b->next.ptr() ) {
        if ( off < b->used )
            return b->data[ off ];
        off -= b->used;
    }
}

Buffer::UC &Buffer::operator[]( SL off ) {
    for( Buffer *b = this; ; b = b->next.ptr() ) {
        if ( off < b->used )
            return b->data[ off ];
        off -= b->used;
    }
}

const Buffer::UC *Buffer::ptr( SL off ) const {
    return &operator[]( off );
}

Buffer::UC *Buffer::ptr( SL off ) {
    return &operator[]( off );
}

Buffer::SL Buffer::size() const {
    SL res = 0;
    for( const Buffer *b = this; b; b = b->next.ptr() )
        res += b->used;
    return res;
}

bool Buffer::empty() const {
    for( const Buffer *b = this; b; b = b->next.ptr() )
        if ( b->used )
            return false;
    return true;
}

Buffer::SL Buffer::item_room() const {
    return item_size - used;
}

void Buffer::cut( SL off ) {
    Ptr<Buffer> t = new Buffer;

    t->used = used - off;
    used = off;

    memcpy( t->data, data + off, t->used );

    t->next = next;
    next = t;
}

void Buffer::copy_to( UC *res ) {
    for( const Buffer *b = this; b; b = b->next.ptr() ) {
        memcpy( res, b->data, b->used );
        res += b->used;
    }
}

void Buffer::skip_some( Ptr<Buffer> &buffer, SL &off_buffer, SL msg_length ) {
    while ( true ) {
        if ( not buffer )
            return;
        if ( msg_length <= buffer->used - off_buffer ) {
            off_buffer += msg_length;
            return;
        }
        msg_length -= buffer->used - off_buffer;
        buffer = buffer->next;
        off_buffer = 0;
    }
}

}
