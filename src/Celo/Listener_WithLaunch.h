#ifndef LISTENER_WITHLAUNCH_H
#define LISTENER_WITHLAUNCH_H

#include "Listener.h"

/**
*/
class Listener_WithLaunch : public Listener {
public:
    Listener_WithLaunch( const char *port );
    Listener_WithLaunch( VtableOnly );
    virtual void launch_browser();
    virtual void rdy();

    // data for rdy (used mainly for debug purpose)
    bool launch;
    const char *port;
    const char *title; ///< if xdotool, look for a window with title
    const char *which_cmd; ///< used to test if a browser is present in the system
    const char *start_url; ///< used to launch a new browser
    const char *xdotool_cmd;
    const char **browser_list;
};


#endif // LISTENER_WITHLAUNCH_H
