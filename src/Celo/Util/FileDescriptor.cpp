#include "FileDescriptor.h"
#include <fcntl.h>

namespace Celo {

int set_non_blocking( int fd ) {
    int flags = fcntl( fd, F_GETFL );
    if ( flags < 0 )
        return flags;
    flags |= O_NONBLOCK;
    return fcntl( fd, F_SETFL, flags );
}

}
