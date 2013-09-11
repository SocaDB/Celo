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


#ifndef CELO_LISTENER_WO_H
#define CELO_LISTENER_WO_H

#include "Util/VoidStruct.h"
#include "Listener.h"

namespace Celo {

/**
*/
template<class ObjWithConnection,class AdditionalData=VoidStruct,bool del=false>
class Listener_WO : public Listener {
public:
    Listener_WO( ObjWithConnection *obj, const char *port, AdditionalData data = AdditionalData() ) : Listener( port ), obj( obj ), data( data ) {
    }

    ~Listener_WO() {
        if ( del )
            delete obj;
    }

    virtual bool connection( int fd ) {
        return obj->connection( this, fd );
    }

    AdditionalData data;

protected:
    ObjWithConnection *obj;
};

}

#endif // CELO_LISTENER_WO_H
