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
