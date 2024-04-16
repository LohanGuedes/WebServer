#ifndef RUNTIME_HPP
#define RUNTIME_HPP
#pragma once
#include "AHttpRequest.hpp"
#include "APollable.hpp"
#include "Logger.hpp"
#include <iostream>
#include <list>
#include <new>
#include <sstream>
#include <sys/epoll.h>
#include <vector>

#define BLOCKING_CHECK -1
#define NONBLOCKING_CHECK 0

class Listener;
class AHttpRequest;
class Client;

class RunTime {
  public:
    // constructors / destructors
    ~RunTime(void);
    static RunTime *getInstance(void) throw();
    static void     deleteInstance(void) throw();

    // variables
    std::vector<Listener const *> listenerPool;
    std::vector<AHttpRequest *>   requestPool;
    std::list<Client *>           clientPool;
    unsigned int                  epollCount;

    // const variables
    // const unsigned long int socketBuffSize;

    // getters / setters
    inline int getEpollInstance() const throw();
    int addListener(std::string const &host, std::string const &port) throw();
    int addListener(Listener const *newListener) throw();
    Listener const *getListener(std::string const &host,
                                std::string const &port) const throw();
    Listener const *getListener(const unsigned int hash) const throw();

    // initialization
    bool createEpollInstance(void) throw();
    bool startListeners(void) const;
    bool addListenersToEpoll(void) throw();

    // event loop
    bool addToEpoll(APollable const *socket) throw();
    bool deleteFromEpoll(const APollable *newInstance) throw();
    bool deleteClient(Client *socket) throw();
    bool checkEpoll(int checkType) const throw();
    bool processRequests(void);

    // cleanup
    bool closeListeners(void) throw();
    void clearRequests(void) throw();
    bool removeRequest(AHttpRequest *find) throw();

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

#endif // !RUNTIME_HPP
