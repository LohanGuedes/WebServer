#include "Listener.hpp"
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

Listener::Listener(std::string const &host, std::string const &port)
    : APollable(Listener::getEpollEventStruct()), host(host),
      port(std::atoi(port.c_str())),
      hostPortHash(Listener::hashStr(host + port)) {

    const int val = 1;

    // get default configs for the listening socket
    if (this->resolveAddr(&this->sockAddrInfo) == false) {
        throw SocketCreateException();
    }
    // create the socket

    if ((this->_fd = socket(this->sockAddrInfo.ss_family,
                            SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0)) ==
        -1) {
        throw SocketCreateException();
    }
    // disable port reserving
    if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) ==
        -1) {
        throw SocketCreateException();
    }

    // bind the socket to an address/port
    if (bind(this->_fd, (struct sockaddr *)&this->sockAddrInfo,
             sizeof(this->sockAddrInfo)) == -1) {
        throw SocketCreateException();
    }
}

Listener::~Listener(void) {
    if (this->_fd != -1)
        close(this->_fd);
    return;
}

void Listener::handlePoll(epoll_event_bitflag bitflag) {
    if (bitflag & EPOLLIN) {
        this->handlePollin();
    }
    if (bitflag & EPOLLOUT) {
        std::cout << "EPOLLOUT hit" << std::endl;
    }
    return;
}

void Listener::listen(void) const {
    // start listening
    if (::listen(this->_fd, SOMAXCONN) == -1) {
        throw SocketListenException();
    }
    return;
}

void Listener::handlePollin(void) {
    RunTime *const runTime = RunTime::getInstance();
    Client        *newClient;
    const int      fd =
        accept4(*this->fd_ptr, NULL, NULL,
                SOCK_NONBLOCK | SOCK_CLOEXEC); // fill these null pointers to
                                               // get info on the peer socket
    if (fd < 0) {
        Logger::log(LOG_ERROR, "Failure on Listener::handlePollin -> accept()");
        return;
    }
    newClient = new Client(fd);
    runTime->addToEpoll(newClient);
    runTime->clientPool.push_back(newClient);

    return;
}

unsigned long Listener::hashStr(std::string const &str) {
    unsigned long hash = 5318; // magic number;
    char          c;

    for (size_t i = 0; i < str.length(); i++) {
        c = str[i];
        hash = ((hash << 5) + hash) + c; // isso é: hash * 33 + c
    }
    return hash;
}

bool Listener::resolveAddr(struct sockaddr_storage *result) const throw() {
    char                   port[10] = {0};
    struct addrinfo const *it;
    struct addrinfo       *head;
    struct addrinfo        hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    snprintf(port, sizeof(port), "%d", this->port); // port number itoa
    if (getaddrinfo(this->host.c_str(), port, &hints, &head) != 0) {
        return (false);
    }
    for (it = head; it != NULL; it = it->ai_next) {
        if (it->ai_socktype == SOCK_STREAM &&
            (it->ai_addrlen == sizeof(struct sockaddr_in) ||
             it->ai_addrlen == sizeof(struct sockaddr_in6)) &&
            (it->ai_family == AF_INET || it->ai_family == AF_INET6)) {
            switch (it->ai_family) {
            case (AF_INET):
                *reinterpret_cast<struct sockaddr_in *>(result) =
                    *reinterpret_cast<struct sockaddr_in *>(it->ai_addr);
                break;
            case (AF_INET6):
                *reinterpret_cast<struct sockaddr_in6 *>(result) =
                    *reinterpret_cast<struct sockaddr_in6 *>(it->ai_addr);
                break;
            }
            freeaddrinfo(head);
            return (true);
        }
    }
    freeaddrinfo(head);
    return (false);
}

struct epoll_event Listener::getEpollEventStruct(void) const throw() {
    return ((struct epoll_event){.events = EPOLLIN | EPOLLET,
                                 .data = {.ptr = (void *)this}});
}
