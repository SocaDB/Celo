#include "BasicHttpRequest_FileServer.h"

BasicHttpRequest_FileServer::BasicHttpRequest_FileServer( int fd ) : BasicHttpRequest( fd ) {
}

void BasicHttpRequest_FileServer::req_GET() {
    if ( send_file( url.data + 1 ) )
        return;

    BasicHttpRequest::req_GET();
}
