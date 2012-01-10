#include "HttpRequest_FileServer.h"
#include "StringHelp.h"

HttpRequest_FileServer::HttpRequest_FileServer( int fd ) : HttpRequest( fd ) {
}

void HttpRequest_FileServer::req_GET() {
    if ( send_file( url.data + 1 ) )
        return;

    HttpRequest::req_GET();
}
