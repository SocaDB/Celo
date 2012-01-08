#ifndef HTTPREQUEST_FILESERVER_H
#define HTTPREQUEST_FILESERVER_H

#include "HttpRequest.h"

/**
*/
class HttpRequest_FileServer : public HttpRequest {
public:
    HttpRequest_FileServer( int fd );
    virtual void req_GET();
};

#endif // HTTPREQUEST_FILESERVER_H
