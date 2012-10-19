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


#include <Celo/StringHelp.h>
#include <Celo/EventLoop.h>
#include <Celo/Signal_WO.h>
#include <Celo/Timer_WO.h>
#include <signal.h>
#include <iostream>

struct MyObserver {
    bool timeout() {
        std::cout << "pouet" << std::endl;
        return false;
    }
    bool signal( int sig ) {
        std::cout << "sig " << sig << std::endl;
        return true;
    }
};


int main() {
    EventLoop el;
    MyObserver mo;

    int sigs[] = { SIGINT, SIGQUIT, SIGKILL, -1 };
    el << new Signal_WO<MyObserver>( &mo, sigs );
    
    el << new Timer_WO<MyObserver>( &mo, 1 );

    return el.run();
}
