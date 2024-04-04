#ifndef RUNTIME_HPP
#define RUNTIME_HPP
#include "APollable.hpp"
#include "Listener.hpp"
#include <iostream>
#include <vector>

class RunTime {
public:
  // constructors / destructors
  ~RunTime(void);
  static RunTime *getInstance(void) throw();
  static void deleteInstance(void) throw();

  std::vector<Listener const *> listenerPool;

  // getters / setters
  int getEpollFd(void) const throw();
  bool startEpollInstance(void);
  bool addToEpoll(const APollable *socket) throw();
  int checkEpoll(void) const throw();

  // exceptions
#if 0
  class EpollCreateException : public std::exception {
  public:
    EpollCreateException(void) : std::exception() {}
    const char *what() const throw() { return ("epoll_create() call failed"); }
  };
#endif

private:
  // private constructor is for the Singleton pattern
  RunTime(void);

  static RunTime *_instance;
  int _epoll_fd;
};

#endif // RunTime
