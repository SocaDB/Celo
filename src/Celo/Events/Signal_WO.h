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


#ifndef Celo_Events_Signal_WO_H
#define Celo_Events_Signal_WO_H

#include "../System/VoidStruct.h"
#include "Signal.h"

namespace Celo {
namespace Events {

/**
*/
template<class ObjWithSignal,class AdditionalData=VoidStruct,bool del=false>
class Signal_WO : public Signal {
public:
    Signal_WO( ObjWithSignal *obj, const int *sigs, AdditionalData data = AdditionalData() ) : Signal( sigs ), data( data ), obj( obj ) {
    }

    ~Signal_WO() {
        if ( del )
            delete obj;
    }

    virtual bool signal( int s ) {
        return obj->signal( this, s );
    }

    AdditionalData data;

protected:
    ObjWithSignal *obj;
};

} // namespace Events
} // namespace Celo

#endif // Celo_Events_Signal_WO_H
