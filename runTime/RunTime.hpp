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
  bool startEpollInstance(void) throw();
  bool checkEpoll(void) const throw();
  bool startListeners(void) const;

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

  bool addToEpoll(const APollable *socket) throw();
  static RunTime *_instance;
  int _epollInstance;
};

#endif // RunTime
