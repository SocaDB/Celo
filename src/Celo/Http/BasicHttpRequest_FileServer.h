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
