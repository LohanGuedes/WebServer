#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "APollable.hpp"
#include "Logger.hpp"

class Client : public APollable {
public:
  Client(int fd);
  virtual ~Client(void);
  virtual void handlePoll(epoll_event_bitflag bitflag);
  virtual struct epoll_event getEpollEventStruct(void) const throw();
#if 0
  HttpRequest	const *request;
#endif
};

#endif // !CLIENT_HPP
