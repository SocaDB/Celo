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


#ifndef EVENTOBJ_H
#define EVENTOBJ_H

#include "Config.h"

/**
*/
class EventObj {
public:
    struct VtableOnly {};

    EventObj( VtableOnly ); ///< a constructor that does not assign any attribute (else than the vtable). Permits to do a new( ptr ) T to change _only_ the vtable (underlying type)
    EventObj( int fd );
    virtual ~EventObj();

    virtual bool inp(); ///< if input data. return true if needs or may accept more.
    virtual bool out(); ///< if ready for output. return true if there is still remaining data to send.
    virtual void err(); ///< if error
    virtual void hup(); ///< if closed
    virtual void rdy(); ///< called after installation of this in event loop


    virtual bool want_poll_inp_at_the_beginning() const;
    virtual bool want_poll_out_at_the_beginning() const;
    virtual bool cnt_default_value() const; ///< false if object is this is to be deleted after a request
    virtual bool called_if_idle() const; ///< true if watching for idle, not for fd

    void poll_out(); ///< add output polling for $this in ev_loop

    class EventLoop *ev_loop;
    int              fd;
};

#endif // EVENTOBJ_H
