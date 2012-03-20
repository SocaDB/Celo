#ifndef EVENTLOOP_H
#define EVENTLOOP_H

/**
*/
class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    int run(); ///< Infinite loop, until stop() is called. return err code < 0 if pb. return 0 if ok.
    void stop(); ///< may be called during an event to say that the infinite loop may stop.

    // additions
    EventLoop &operator<<( class EventObj *ev_obj ); ///< add a event. Thread safe

    // modifications
    void mod( class EventObj *ev_obj, bool want_out = false ); ///< fd will be polled by ev_obj
    void poll_out( class EventObj *ev_obj ); ///< poll ev_obj for output

protected:
    bool cnt; ///< continue ?
    int event_fd; ///< file
};

#endif // EVENTLOOP_H
