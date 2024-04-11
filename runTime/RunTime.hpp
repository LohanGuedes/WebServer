#ifndef RUNTIME_HPP
#define RUNTIME_HPP
#include "APollable.hpp"
#include "Client.hpp"
#include "Listener.hpp"
#include <iostream>
#include <list>
#include <vector>

#define BLOCKING_CHECK -1
#define NONBLOCKING_CHECK 0

class RunTime {
  public:
    // constructors / destructors
    ~RunTime(void);
    static RunTime *getInstance(void) throw();
    static void     deleteInstance(void) throw();

    // variables
    std::vector<Listener const *> listenerPool;
    std::vector<int>              requestPool;
    std::list<Client const *>     clientPool;
    unsigned int                  epollCount;

    // getters / setters
    inline int getEpollInstance() const throw();
    int        addListener(std::string const &host, std::string const &port);
    int        addListener(Listener const *newListener);

    // initialization
    bool createEpollInstance(void) throw();
    bool startListeners(void) const;
    bool addListenersToEpoll(void) throw();

    // event loop
    bool addToEpoll(const APollable *socket) throw();
    bool checkEpoll(int checkType) const throw();
    bool processRequests(void);

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
    int             _epollInstance;
};

#endif // RunTime
