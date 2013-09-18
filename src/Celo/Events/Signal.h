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


#ifndef Celo_Events_Signal_H
#define Celo_Events_Signal_H

#include "Event.h"

namespace Celo {
namespace Events {

/**
*/
class Signal : public Event {
public:
    Signal( const int *sigs );

protected:
    virtual void inp();
    virtual bool signal( int s ) = 0; ///< return true to continue watching for signal
};

} // namespace Events
} // namespace Celo

#endif // Celo_Events_Signal_H
