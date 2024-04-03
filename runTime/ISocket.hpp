#pragma once
#ifndef ISOCKET_HPP
#define ISOCKET_HPP
#include <stdint.h>
#include <sys/epoll.h>

typedef uint32_t epoll_event_bitflag;

class ISocket {
public:
  ISocket(void) {}
  virtual ~ISocket(void) {}
  virtual void handlePoll(epoll_event_bitflag bitflag) = 0;

protected:
  virtual void handlePollin(epoll_event_bitflag bitflag) = 0;
  virtual void handlePollout(void) = 0;
};

#endif // !ISOCKET_HPP
