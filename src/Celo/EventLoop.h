#ifndef EVENTLOOP_H
#define EVENTLOOP_H

/**
*/
class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    int run(); ///< Infinite loop, until stop() is called. return err code < 0 if pb. return 0 if ok.
    void stop(); ///< may called during an event to say that the infinite loop may stop.

    EventLoop &operator<<( class EventObj *ev_obj ); ///< add a event
    void poll_out( class EventObj *ev_obj ); ///<

protected:
    bool cnt; ///< continue ?
    int event_fd; ///< file
};

#endif // EVENTLOOP_H
