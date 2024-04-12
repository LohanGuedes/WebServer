#pragma once
#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "APollable.hpp"
#include "ServerConfig.hpp"
#include <arpa/inet.h>
#include <exception>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Listener : public APollable {
  public:
    // constructors / destructors
    Listener(std::string const &host, std::string const &port);
    virtual ~Listener(void);

    // methods
    virtual void               handlePoll(epoll_event_bitflag bitflag);
    virtual struct epoll_event getEpollEventStruct(void) const throw();
    void                       listen(void) const;
    void                       handlePollin(void);

    // aux methods
    static unsigned long hashStr(std::string const &str);
    bool                 resolveAddr(struct sockaddr_in *ret) const throw();

    // fields
    const std::string   host;
    const int           port;
    const unsigned long hostPortHash;

#if 0
		std::vector<ServerProps>		propsPool;
#endif

    // Exceptions
    class SocketCreateException : public std::exception {
      public:
        SocketCreateException(void) : std::exception() {}
        const char *what() const throw() {
            return ("socket() function failed");
        }
    };
    class SocketListenException : public std::exception {
      public:
        SocketListenException(void) : std::exception() {}
        const char *what() const throw() {
            return ("listen() function failed");
        }
    };
};

#endif // !LISTENER_HPP
