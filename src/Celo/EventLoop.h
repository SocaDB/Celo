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

    // loop
    int run(); ///< Infinite loop, until stop() is called. return err code < 0 if pb. return ret_val (sent by this->stop) if ok.
    void stop( int ret_val = 0 ); ///< may be called during a callback to please the infinite loop to gracefully stop after that.

    // additions of event objects
    EventLoop &operator<<( Events::Event *ev_obj ); ///< add a event. Thread safe

    // suppressions of event objects
    EventLoop &operator>>( Events::Event *ev_obj ); ///< not thread safe

    // modifications
    void poll_out_obj( Events::Event *ev_obj ); ///< look for ev_obj availability for output

protected:
    Events::Event *last_ev_to_del; ///<
    int            event_fd; ///< for epoll
    int            ret;
    bool           cnt; ///< continue ?
};

}

#endif // CELO_EVENTLOOP_H
