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


#ifndef EVENTLOOP_H
#define EVENTLOOP_H

class EventObj;
class IdleObj;

/**
*/
class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    int run(); ///< Infinite loop, until stop() is called. return err code < 0 if pb. return 0 if ok.
    void stop(); ///< may be called during an event to say that the infinite loop may stop.

    // additions
    EventLoop &operator<<( EventObj *ev_obj ); ///< add a event. Thread safe
    EventLoop &operator<<( IdleObj *ev_obj ); ///< add a "idle" event. Thread safe

    // suppressions
    EventLoop &operator>>( EventObj *ev_obj ); ///< not thread safe
    EventLoop &operator>>( IdleObj *ev_obj ); ///< not thread safe

    // modifications
    void mod( EventObj *ev_obj, bool want_out = false ); ///< fd will be polled by ev_obj
    void poll_out( EventObj *ev_obj ); ///< poll ev_obj for output

protected:
    friend class IdleObj;
    IdleObj *idle_list; ///< list of idle objects
    int event_fd; ///< file
    bool cnt; ///< continue ?
};

#endif // EVENTLOOP_H
