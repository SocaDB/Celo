#ifndef HTTPREQUEST_FILESERVER_H
#define HTTPREQUEST_FILESERVER_H

#include "HttpRequest.h"

/**
*/
class HttpRequest_FileServer : public HttpRequest {
public:
    HttpRequest_FileServer( int fd, const char *base );
    virtual void req();

    const char *base;
};

#endif // HTTPREQUEST_FILESERVER_H
