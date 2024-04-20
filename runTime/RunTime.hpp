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

    // VARIABLES //
    /* A vector of all Listeners */
    std::vector<Listener const *> listenerPool;
    /* A vector of all the currently active requests */
    std::vector<AHttpRequest *> requestPool;
    /* The list of currently active clients */
    std::list<Client *> clientPool;
    /* The ammount of APollables currently subscribed to the epoll instance*/
    unsigned int epollCount;

    // CONST VARIABLES //
    // const unsigned long int socketBuffSize;

    // GETTERS / SETTERS //

    /*
     * Returns the internal epoll instance fd.
     * Params:
     * @ none
     * Returns:
     * @ -1: The epoll instance hasn't been initialized yet.
     * @ >0: A valid epoll instance.
     */
    inline int getEpollInstance(void) const throw();

    /*
     * Checks if there is a listener in the listenerPool with the same
     * host/port, if not, creates a new Listener object and adds it to the
     * listenerPool.
     * It creates a new Listener object and adds it to the listenerPool.
     * Params:
     * @ host -> The host for the 'host/port' pair.
     * @ port -> The port for the 'host/port' pair.
     * Returns:
     * @ 1: This host/port pair is already in the listenerPool.
     * @ 0: A new Listener was created and added to the listenerPool.
     * @ -1: Error on creating the listener for the host/port pair.
     * or NULL if it didn't find any listener with the given 'hash'.
     */
    int addListener(std::string const &host, std::string const &port) throw();

    /*
     * Checks if there is a listener in the listenerPool with the same
     * host/port, if not, adds the 'newListener' to the listenerPool.
     * It doesn't create a new object, the pointer is added to the listenerPool.
     * Params:
     * @ newListener -> The listener to be inserted.
     * Returns:
     * @ 1: The listener is already in the listenerPool.
     * @ 0: The listener was added to the listenerPool.
     * @ -1: Error on adding the listener to the listenerPool.
     * or NULL if it didn't find any listener with the given 'hash'.
     */
    int addListener(Listener const *newListener) throw();

    /*
     * Finds the listener in the listenerPool that has the same 'host/port'.
     * Params:
     * @ host -> The host for the 'host/port' pair.
     * @ port -> The port for the 'host/port' pair.
     * Returns:
     * @ A pointer to the instance found,
     * or NULL if it didn't find any listener with the given 'hash'.
     */
    Listener const *getListener(std::string const &host,
                                std::string const &port) const throw();

    /*
     * Finds the listener in the listenerPool that matches the 'hash'
     * Params:
     * @ hash -> the hash to be looked for.
     * Returns:
     * @ A pointer to the instance found,
     * or NULL if it didn't find any listener with the given 'hash'.
     */
    Listener const *getListener(const unsigned int hash) const throw();

    // RUNTIME INITIALIZATION //

    /*
     * Creates an epoll instance if it doesn't already exists.
     * Params:
     * @ none.
     * Returns:
     * @ true -> the instance was created or already existed.
     * @ false -> an error occured.
     */
    bool createEpollInstance(void) throw();

    /*
     * Starts all Listeners on the listenerPool.
     * It's has the same effect of calling listen() on a passive socket.
     * Params:
     * @ none.
     * Returns:
     * @ true -> TBD.
     * @ false -> TBD.
     */
    bool startListeners(void) const;

    /*
     * Subscribes all listeners in the listenerPool to the epoll instance.
     * Params:
     * @ none.
     * Returns:
     * @ true -> TBD.
     * @ false -> TBD.
     */
    bool addListenersToEpoll(void) throw();

    // MAIN EVENT LOOP //

    /*
     * Adds 'socket' to the epoll instance to be observed for new events.
     * Params:
     * @ newInstance -> The object to be monitored for events.
     * Returns:
     * @ true -> TBD.
     * @ false -> TBD.
     */
    bool addToEpoll(APollable const *socket) throw();

    /*
     * Removes 'socket' from the epoll instance.
     * Params:
     * @ newInstance -> The object to be deleted from the epoll.
     * Returns:
     * @ true -> TBD.
     * @ false -> TBD.
     */
    bool deleteFromEpoll(const APollable *newInstance) throw();

    /*
     * Try to find the given Client on the CLientPool, if it exists,
     * pop it from the CLientPool, remove it from the epoll instance,
     * and pop it's associated request from the requestPool.
     * Params:
     * @ socket -> The socket to be deleted.
     * Returns:
     * true -> The Client was successfully deleted.
     * false -> The Client wasn't found.
     */
    bool deleteClient(Client *socket) throw();

    /*
     * Check the epoll instance for any new events,
     * and run the handlePoll() of each APollable instance subscribed in
     * this RunTime.
     * Params:
     * @ checkType -> Blocking or non-blocking check on epoll.
     * Returns:
     * true -> TBD.
     * false -> TBD.
     */
    bool checkEpoll(int checkType) const throw();

    /*
     * Run the processRequest() of each request in the requestPool.
     * Params:
     * @ none
     * Returns:
     * true -> TBD.
     * false -> TBD.
     */
    bool processRequests(void);

    // CLEANUP //

    /*
     * Properly close every Listener on the listenerPool.
     * Params:
     * @ none
     * Returns:
     * true -> TBD.
     * false -> TBD.
     */
    bool closeListeners(void) throw();

    /*
     * Delete every Request on the requestPool.
     * Params:
     * @ none
     * Returns:
     * @ none
     */
    void clearRequests(void) throw();

    /*
     * Removes 'find' from the requestPool.
     * Params:
     * @ find - > the Request object to be removed from the requestPool.
     * Returns:
     * @ true -> 'find' was successfully removed from the requestPool.
     * @ false -> 'find' didn't exist on the requestPool.
     */
    bool removeRequest(AHttpRequest *find) throw();

    // EXCEPTIONS //
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
