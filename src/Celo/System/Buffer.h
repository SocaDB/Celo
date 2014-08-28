#ifndef BUFFER_H
#define BUFFER_H

#include "TypeConfig.h"
#include "Stream.h"
#include "Ptr.h"

namespace Celo {

/**
*/
class Buffer : public ObjectWithCptUse {
public:
    enum { item_size = 1024 - sizeof( Ptr<Buffer> ) - sizeof( SI32 ) };

    Buffer();
    void write_to_stream( Stream &os ) const;

    const PI8 &operator[]( ST off ) const;
    PI8 &operator[]( ST off );

    const PI8 *ptr( ST off ) const;
    PI8 *ptr( ST off );

    ST size() const; ///< total size, starting from this and using next
    bool empty() const;

    ST item_room() const; ///< available room in this (`next` excluded)

    void cut( int off ); ///< split this into two buffer (store the second in next). Mainly for test purpose

    void copy_to( PI8 *res );

    // attributes
    Ptr<Buffer> next;
    SI32        used;
    PI8         data[ item_size ];
};

}

#endif // BUFFER_H
