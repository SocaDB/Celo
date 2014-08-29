#ifndef BUFFER_H
#define BUFFER_H

#include "Ptr.h"

namespace Celo {

/**
*/
class Buffer : public ObjectWithCptUse {
public:
    typedef unsigned char PI8;
    typedef int           SI32;
    typedef long long     SI64;

    enum { item_size = 1024 - sizeof( Ptr<Buffer> ) - sizeof( SI32 ) };

    Buffer();

    template<class Stream>
    void write_to_stream( Stream &os ) const {
        for( int i = 0; i < size(); ++i ) {
            static const char *c = "0123456789abcdef";
            os << ( i ? " " : "" ) << c[ (*this)[ i ] / 16 ] << c[ (*this)[ i ] % 16 ];
        }
    }

    const PI8 &operator[]( SI64 off ) const;
    PI8 &operator[]( SI64 off );

    const PI8 *ptr( SI64 off ) const;
    PI8 *ptr( SI64 off );

    SI64 size() const; ///< total size, starting from this and using next
    bool empty() const;

    SI64 item_room() const; ///< available room in this (`next` excluded)

    void cut( int off ); ///< split this into two buffer (store the second in next). Mainly for test purpose

    void copy_to( PI8 *res );

    // attributes
    Ptr<Buffer> next;
    SI32        used;
    PI8         data[ item_size ];
};

}

#endif // BUFFER_H
