#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

//#include "StringBuf.h"
#include "StringBlk.h"
#include "EventObj.h"

/**
*/
class HttpRequest : public EventObj {
public:
    enum {
        size_buff = 1024 - sizeof( void * ) - sizeof( EventObj )
    };

    typedef enum {
        GET,
        POST,
        PUT,
        DELETE
    } ReqType;

    HttpRequest( int fd );
    virtual ~HttpRequest();

    virtual bool inp(); ///< if input data
    virtual bool out(); ///< if ready for output
    virtual void err(); ///< if error
    virtual void hup(); ///< if closed

protected:
    bool inp( char *data, const char *end );

    #define ERR( NUM, MSG ) bool error_##NUM();
    #include "ErrorCodes.h"
    #undef ERR

    int cur_proc; ///< current reading / writing procedure
    ReqType req_type;
    std::string tmp;  ///< used in procs
};

#endif // HTTPREQUEST_H
