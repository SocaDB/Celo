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

#include "BrowserLaunchListener.h"
#include "../Util/StringHelp.h"
#include <stdlib.h>
#include <sstream>
using namespace std;

namespace Celo {
namespace Events {

static const char *_browser_list_prop[] = {
    "chromium-browser",
    "google-chrome",
    "firefox",
    0
};

static bool _has_cmd( const char *which, const char *exe ) {
    if ( which == 0 or exe == 0 )
        return false;
    string cmd = string( which ) + ' ' + exe + " > /dev/null";
    return system( cmd.c_str() ) == 0;
}


BrowserLaunchListener::BrowserLaunchListener( const char *port ) : Listener( port ), port( port ) {
    title = 0;
    launch = false;
    start_url = "";
    which_cmd = "which";
    xdotool_cmd = "xdotool";
    browser_list = _browser_list_prop;
}

BrowserLaunchListener::BrowserLaunchListener( VtableOnly vo ) : Listener( vo ) {
}

void BrowserLaunchListener::launch_browser() {
    // try with xdotool
    if ( title and xdotool_cmd ) {
        string cmd = string( xdotool_cmd ) + " search " + title + " windowactivate key F5";
        if ( system( cmd.c_str() ) == 0 ) {
            std::cout << "Call to xdotool (looking for '" << title << "') has succeeded." << std::endl;
            return;
        }
    }
    std::cout << "Call to xdotool (looking for '" << title << "') has failed." << std::endl;

    // else, try to launch in a new browser
    std::ostringstream cmd;
    for( int i = 0; browser_list[ i ]; ++i ) {
        if ( _has_cmd( which_cmd, browser_list[ i ] ) ) {
            string cmd = string( browser_list[ i ] ) + " http://localhost:" + port + start_url + " &";
            if ( system( cmd.c_str() ) == 0 )
                return;
        }
    }
}

void BrowserLaunchListener::rdy() {
    if ( launch )
        launch_browser();
}

} // namespace Celo
} // namespace Events
