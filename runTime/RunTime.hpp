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

  // variables
  std::vector<Listener const *> listenerPool;
  unsigned int epollCount;

  // getters / setters
  inline int getEpollInstance() const throw();

  // initialization
  bool createEpollInstance(void) throw();
  bool startListeners(void) const;
  bool addListenersToEpoll(void) throw();

  // event loop
  bool addToEpoll(const APollable *socket) throw();
  bool checkEpoll(void) const throw();

  // cleanup
  bool closeListeners(void) throw();

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
  int _epollInstance;
};

#endif // RunTime
