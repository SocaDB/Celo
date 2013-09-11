/*
 Copyright 2012 Structure Computation  www.structure-computation.com
 Copyright 2012 Hugo Leclerc

 This file is part of Celo.

 Celo is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Celo is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with Celo. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef BASICBasicHttpRequest_H
#define BASICBasicHttpRequest_H

#include "EventObj_WO.h"
#include "StringCpn.h"

/**
*/
class BasicHttpRequest : public EventObj_WO {
public:
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

    BasicHttpRequest( int fd );
    virtual ~BasicHttpRequest();

protected:
    virtual bool inp( char *data, const char *end );

    // BasicHttpRequest methods that may be redefined
    virtual void req_GET(); ///< Called after parsing of the header. By default, look up for files in base_dir()
    virtual void req_PUT( char *beg, ST len ); ///< Called after parsing of the header. By default, look up for files in base_dir()
    virtual void req_POST( char *beg, ST len ); ///< Called after parsing of the header. By default, look up for files in base_dir()

    // helpers
    bool send_file( const char *url ); ///< send http header and file data. return false if file does not exist
    void send_head( const char *url ); ///< send corresponding http header

    #define ERR( NUM, MSG ) void error_##NUM();
    #include "ErrorCodes.h"
    #undef ERR

    // parsing context
    void *inp_cont; ///< for continuation

    // output from the parser
    int       content_length; ///<
    StringCpn url;            ///< requested url
    StringCpn cookies;        ///< requested url
};

#endif // BASICBasicHttpRequest_H
