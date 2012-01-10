#include "StringHelp.h"
#include "EventLoop.h"
#include "EventObj.h"

#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>

EventLoop::EventLoop() {
    // epoll init
    event_fd = epoll_create1( 0 );
    if ( event_fd < 0 )
        perror( "epoll_create1" );
}

EventLoop::~EventLoop() {
    close( event_fd );
}

int EventLoop::run() {
    //
    cnt = true;

    // loop
    const int max_events = 64;
    epoll_event events[ max_events ];
    while( cnt ) {
        int nfds = epoll_wait( event_fd, events, max_events, -1 );
        if ( nfds == -1 ) {
            perror( "epoll_wait" );
            return -1;
        }

        // for each event
        for( int n = 0; n < nfds; ++n ) {
            EventObj *rq = reinterpret_cast<EventObj *>( events[ n ].data.ptr );
            bool cnt = false;

            if ( events[ n ].events & EPOLLIN ) // input data
                cnt |= rq->inp();

            if ( events[ n ].events & EPOLLOUT ) // output data
                cnt |= rq->out();

            if ( events[ n ].events & EPOLLHUP ) { // end of the bananas ?
                rq->hup();
                cnt = false;
            }

            if ( events[ n ].events & EPOLLERR ) { // error ?
                rq->err();
                cnt = false;
            }

            if ( not cnt )
                delete rq;
        }
    }

    return 0;
}

void EventLoop::stop() {
    cnt = false;
}

EventLoop &EventLoop::operator<<( class EventObj *ev_obj ) {
    if ( not ev_obj )
        return *this;
    if ( ev_obj->fd < 0 ) {
        delete ev_obj;
        return *this;
    }

    ev_obj->ev_loop = this;

    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET; // no EPOLLOUT by default (set up e.g. if write not completed)
    ev.data.u64 = 0; // for valgrind on 32 bits machines
    ev.data.ptr = ev_obj;
    if ( epoll_ctl( event_fd, EPOLL_CTL_ADD, ev_obj->fd, &ev ) == -1 )
        perror( "epoll_ctl add" );

    ev_obj->rdy();
    return *this;
}

void EventLoop::poll_out( class EventObj *ev_obj ) {
    PRINT( ev_obj->fd );
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLOUT;
    ev.data.u64 = 0; // for valgrind on 32 bits machines
    ev.data.ptr = ev_obj;
    if ( epoll_ctl( event_fd, EPOLL_CTL_MOD, ev_obj->fd, &ev ) == -1 )
        perror( "epoll_ctl mod" );
}

