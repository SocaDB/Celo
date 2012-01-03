#include "HttpRequest_FileServer.h"

HttpRequest_FileServer::HttpRequest_FileServer( int fd ) : HttpRequest( fd ) {
}

void HttpRequest_FileServer::req() {
    if ( send_file( url_data + 1 ) )
        return;

    HttpRequest::req();
}
