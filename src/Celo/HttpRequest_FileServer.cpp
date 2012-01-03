#include "HttpRequest_FileServer.h"

HttpRequest_FileServer::HttpRequest_FileServer( int fd, const char *base ) : HttpRequest( fd ), base( base ) {
}

void HttpRequest_FileServer::req() {
    std::string file = base + std::string( url_dat );
    if ( send_file( file.c_str() ) )
        return;

    // not found
    error_404();
}
