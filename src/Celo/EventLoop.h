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
