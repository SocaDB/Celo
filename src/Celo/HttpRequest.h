#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "EventObj_WO.h"
#include "StringBlk.h"

/**
*/
class HttpRequest : public EventObj_WO {
public:
    enum {
        size_buff = 1024 - sizeof( void * ) - sizeof( EventObj )
    };

    typedef enum {
        GET,
        POST,
        PUT,
        DELETE,
        OPTIONS,
        HEAD,
        TRACE,
        CONNECT
    } ReqType;

    HttpRequest( int fd );
    virtual ~HttpRequest();

    // EventObj
    virtual void inp(); ///< if input data
    virtual bool end(); ///< return true if done

    // HttpRequest methods that may be redefined
    virtual void req(); ///< Called after parsing of the header. By default, look up for files in base_dir()

protected:
    bool send_file( const char *url ); ///< return true of file exists
    void send_head( const char *url ); ///< send corresponding http header

    void inp( char *data, const char *end ); ///< > 0 means continuation, < 0 means error, 0 means end

    #define ERR( NUM, MSG ) void error_##NUM();
    #include "ErrorCodes.h"
    #undef ERR

    // parsing context
    void *inp_cont; ///< for continuation

    // output from the parser
    ReqType req_type; ///< GET / ...
    char   *url_data; ///< requested url, terminated by \0
    int     url_size; ///<
    int     url_rese; ///< used only if continuation

    friend void test_HttpRequest( const char *data );
};

#endif // HTTPREQUEST_H
