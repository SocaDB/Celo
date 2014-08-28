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


#ifndef CELO_EVENTLOOP_H
#define CELO_EVENTLOOP_H

namespace Celo {

namespace Events {
class Event;
}

/**
*/
class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    /// Infinite loop, until stop() is called. return err code < 0 if pb. return ret_val (the value that is sent by this->stop()) if ok.
    int run();

    /// Procedure to be called to gracefully exit from run().
    /// May be called e.g. during a callback
    void stop( int ret_val = 0 );

    /// add a to be watched event object.
    /// ev_obj may be deleted inside the loop if it says that everything is done with it.
    /// thread safe.
    EventLoop &operator<<( Events::Event *ev_obj ); ///< add an event. Thread safe

    /// suppressions of event objects
    /// NOT thread safe
    EventLoop &operator>>( Events::Event *ev_obj ); ///< not thread safe

    /// look for ev_obj availability for output
    void poll_out_obj( Events::Event *ev_obj );

protected:
    friend class Events::Event;

    Events::Event *last_ev_to_del; ///<
    int            event_fd; ///< for epoll
    int            ret;
    bool           cnt; ///< continue ?
};

}

#endif // CELO_EVENTLOOP_H
