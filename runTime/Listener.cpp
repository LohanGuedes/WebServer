#include "Listener.hpp"
#include "APollable.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "RunTime.hpp"
#include <arpa/inet.h>
#include <list>
#include <sys/epoll.h>
#include <sys/socket.h>

Listener::Listener(std::string host, int port)
    : APollable(Listener::getEpollEventStruct()), host(host), port(port) {

    const int                val = 1;
    const struct sockaddr_in conf = {
        .sin_family = AF_INET,
        .sin_port = ntohs(port),
        .sin_addr = {.s_addr = inet_addr(host.c_str())},
        .sin_zero = {0}};

    // create the socket
    if ((this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK,
                            0)) == -1) {
        throw SocketCreateException();
    }
    // disable port reserving
    if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) ==
        -1) {
        throw SocketCreateException();
    }

    if (bind(this->_fd, (struct sockaddr *)&conf, sizeof(struct sockaddr_in)) ==
        -1) {
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
    const Client  *newClient;
    const int      fd =
        accept(*this->fd_ptr, NULL,
               NULL); // fill these null pointers to get info on the peer socket
    if (fd < 0) {
        Logger::log(LOG_ERROR, "Failure on Listener handlePollin in accept()");
        return;
    }
    newClient = new Client(fd);
    runTime->addToEpoll(newClient);
    runTime->clientPool.push_back(newClient);

    return;
}

struct epoll_event Listener::getEpollEventStruct(void) const throw() {
    return ((struct epoll_event){.events = EPOLLIN | EPOLLET,
                                 .data = {.ptr = (void *)this}});
}
