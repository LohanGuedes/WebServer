#pragma once
#include <sys/epoll.h>
#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "ISocket.hpp"
#include "ServerConfig.hpp"
#include <arpa/inet.h>
#include <exception>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Listener : public ISocket {
public:
  // constructors / destructors
  Listener(std::string address, int port);
#if 0
		Listener(Listener const &cpy);
		Listener&	operator=(Listener const &rhs);
#endif
  virtual ~Listener(void);

  // fields
  const struct epoll_event epoll_conf;
  static std::vector<Listener *> listenerPool;

  // methods
  virtual void handlePoll(epoll_event_bitflag bitflag);
  int getFd(void);
  void listen(void);
  static struct epoll_event get_epoll_event(Listener *addr);

#if 0
		std::vector<ServerConfig>		configPool;
#endif

  // Exceptions
  class SocketCreateException : public std::exception {
  public:
    SocketCreateException(void) : std::exception() {}
    const char *what() const throw() { return ("socket() function failed"); }
  };
  class SocketListenException : public std::exception {
  public:
    SocketListenException(void) : std::exception() {}
    const char *what() const throw() { return ("listen() function failed"); }
  };

private:
  virtual void handlePollin(epoll_event_bitflag bitflag);
  virtual void handlePollout(void);
  int _fd;
};

#endif // !LISTENER_HPP
