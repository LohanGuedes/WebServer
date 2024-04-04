#pragma once
#ifndef APOLLABLE_HPP
#define APOLLABLE_HPP
#include <stdint.h>
#include <sys/epoll.h>

typedef uint32_t epoll_event_bitflag;

class APollable {
public:
  APollable(const struct epoll_event events)
      : fd_ptr(&this->_fd), _events(events) {}
  virtual ~APollable(void) {}
  virtual void handlePoll(epoll_event_bitflag bitflag) = 0;
  virtual struct epoll_event getEpollEventStruct(void) const throw() = 0;

  const int *const fd_ptr;

protected:
  int _fd;
  const struct epoll_event _events;
};

#endif // !ISOCKET_HPP
