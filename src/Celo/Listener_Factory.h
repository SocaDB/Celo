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


#ifndef LISTENER_FACTORY_H
#define LISTENER_FACTORY_H

#include "Util/VoidStruct.h"
#include "Listener.h"

namespace Celo {

/**
  Listener that creates an object of type EO (with the corresponding fd, and data as constructor arguments) each time there's a new connection
*/
template<class EO,class AdditionnalData=VoidStruct,class PA=Listener>
class Listener_Factory : public PA {
public:
    Listener_Factory( const char *port, AdditionnalData data = AdditionnalData() ) : PA( port ), data( data ) {}
    Listener_Factory( typename PA::VtableOnly vo ) : PA( vo ) {}

    virtual bool connection( int fd ) {
        *PA::ev_loop << new EO( data, fd );
        return true;
    }

    AdditionnalData data;
};

}

#endif // LISTENER_FACTORY_H
