#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "StringBuf.h"
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
        READ_REQ_END = 0,
        READ_REQ_TYPE,
        READ_REQ_TYPE_B,
          READ_REQ_TYPE_BE, READ_REQ_TYPE_BEG, READ_REQ_TYPE_BEG_,
        READ_REQ_TYPE_P,
          READ_REQ_TYPE_PO, READ_REQ_TYPE_POS, READ_REQ_TYPE_POST, READ_REQ_TYPE_POST_,
          READ_REQ_TYPE_PU, READ_REQ_TYPE_PUT, READ_REQ_TYPE_PUT_,

        READ_REQ_TYPE_URL,
        READ_REQ_BEG_ERR // must be at the end
    } ProcNum;

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
    int inp( char *&data, const char *end );

    #define ERR( NUM, MSG ) int error_##NUM();
    #include "ErrorCodes.h"
    #undef ERR

    ProcNum cur_proc; ///< current reading / writing procedure
    ReqType req_type;
    std::string tmp;  ///< used in procs
};

#endif // HTTPREQUEST_H
