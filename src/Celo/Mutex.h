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


#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

/**
@brief Mutual Exclusion.

Used for multi-threading to protect a ressource.

Only on thread can obtain a lock(). So, one can say that code between a lock() and a free() is protected.
*/
class Mutex {
public:
    Mutex();
    ~Mutex();

    int  try_lock(); ///< return 0 if acquired
    void lock();
    void free();

private:
    friend class WaitCondition;
    pthread_mutex_t mutex;
};

/**
  call free on mutex at the end of the current lexical scope
*/
struct AutoMutexFree {
    AutoMutexFree( Mutex &mutex ) : mutex( mutex ) {}
    ~AutoMutexFree() { mutex.free(); }
    Mutex &mutex;
};

#endif // MUTEX_H
