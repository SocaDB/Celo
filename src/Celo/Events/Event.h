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


#ifndef Celo_Events_Event_H
#define Celo_Events_Event_H

#include "../System/TypeConfig.h"

namespace Celo {
class EventLoop;
namespace Events {

/**
*/
class Event {
public:
    struct VtableOnly {};

    Event( VtableOnly ); ///< a constructor that does not assign any attribute (else than the vtable). Permits to do a new( ptr ) T to change _only_ the vtable (underlying type)
    Event( int fd );
    virtual ~Event(); ///< destructor of child classes may read information from the state attribute

    // mod for this
    void poll_out(); ///< add output polling for *this in ev_loop

    // public attributes
    int        fd;      ///< file descriptor
    EventLoop *ev_loop; ///< the event_loop where this is registered

protected:
    enum {
        WF_more_inp = 1,
        WF_more_out = 2,

        _last_Event_WF = 2
    };
    enum {
        EF_generic_error = 1, ///< generic error. TODO: more accurate information
        EF_rd_hup_error = 2,
        EF_hup_error = 4,

        _last_Event_EF = 4
    };

    friend class Celo::EventLoop;

    virtual void inp(); ///< if input data. return true if needs or may accept more.
    virtual void out(); ///< if ready for output. return true if there is still remaining data to send.
    virtual void rdy(); ///< called after installation of this in event loop

    void wait_for_more_inp() { waiting |= WF_more_inp; }
    void wait_for_more_out( bool need_poll_out = true ) { waiting |= WF_more_out; if ( need_poll_out ) poll_out(); }

    void rd_hup_error() { errors |= EF_rd_hup_error; reg_for_deletion(); }
    void hup_error() { errors |= EF_hup_error; reg_for_deletion(); }

    void reg_for_deletion();

    // attributes
    Event     *next_ev_to_del;
    int        waiting; ///< combinations of WF_...
    int        errors;  ///< combinations of EF_...
};

} // namespace Events
} // namespace Celo

#endif // Celo_Events_Event_H
