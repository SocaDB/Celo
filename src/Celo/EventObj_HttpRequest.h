#ifndef EVENTOBJ_HTTPREQUEST_H
#define EVENTOBJ_HTTPREQUEST_H

#include "EventObj_WO.h"

/**
*/
class EventObj_HttpRequest : public EventObj_WO {
public:
    EventObj_HttpRequest( VtableOnly );
    EventObj_HttpRequest( int fd );

protected:
    bool send_file( const char *url ); ///< helper
    void send_head( const char *url ); ///< helper

    #define ERR( NUM, MSG ) void error_##NUM();
    #include "ErrorCodes.h"
    #undef ERR
};

#endif // EVENTOBJ_HTTPREQUEST_H
