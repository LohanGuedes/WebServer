#include "Client.hpp"

Client::Client(int const fd) : APollable(this->getEpollEventStruct()) {
    this->_fd = fd;
}

Client::~Client(void) {}

void Client::handlePoll(epoll_event_bitflag const bitflag) {
    if (bitflag & EPOLLIN) {
        Logger::log(LOG_WARNING, "todo: handle Pollin for Client");
        return;
    }
    return;
}

struct epoll_event Client::getEpollEventStruct(void) const throw() {
    return ((const epoll_event){
        .events = EPOLLIN | EPOLLOUT | EPOLLET,
        .data = {.ptr = (void *)this},
    });
}
