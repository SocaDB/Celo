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


#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

/**
  @brief new thread...

  Example:
@code
struct Toto : public Thread {
    virtual void run() { sleep( 1 ); }
};
int main() {
    Toto toto;
    toto.exec();
    ... // do some stuff in parallel of toto.run()
    toto.wait(); // wait until toto has finished
}
@endcode
*/
class Thread {
public:
    virtual ~Thread();
    virtual void run() = 0;

    void exec(); ///< call run() in a new separate thread
    void wait(); ///< wait until run is finished (after an exec call)

private:
    pthread_t thread;
};

#endif // THREAD_H
