#ifndef BASICHTTPREQUEST_FILESERVER_H
#define BASICHTTPREQUEST_FILESERVER_H

#include "BasicHttpRequest.h"

/**
*/
class BasicHttpRequest_FileServer : public BasicHttpRequest {
public:
    BasicHttpRequest_FileServer( int fd );
    virtual void req_GET();
};

#endif // BASICHTTPREQUEST_FILESERVER_H
