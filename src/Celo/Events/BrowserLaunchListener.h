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


#ifndef Celo_Events_BrowserLaunchListener_H
#define Celo_Events_BrowserLaunchListener_H

#include "Listener.h"

namespace Celo {
namespace Events {


/**
*/
class BrowserLaunchListener : public Listener {
public:
    BrowserLaunchListener( const char *port );
    BrowserLaunchListener( VtableOnly );
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

} // namespace Celo
} // namespace Events

#endif // Celo_Events_BrowserLaunchListener_H
