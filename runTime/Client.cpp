#include "Client.hpp"
#include "RunTime.hpp"

Client::Client(int const fd) : APollable(this->getEpollEventStruct()) {
    this->_fd = fd;
}

Client::~Client(void) {}

void Client::handlePoll(epoll_event_bitflag const bitflag) {
    AHttpRequest *req;
    //    char          buff[1000]; // just for test
    RunTime *rt;

    if (bitflag & EPOLLIN) {
        Logger::log(LOG_WARNING, "Mocked Pollin on Client");
        req = this->parseHeader();
        if (!req) {
            ; // send bad request response
        }
#if 0
        while (recv(this->_fd, buff, sizeof(buff), 0) != 0)
            ; // just to empty the buffer and trigger a pollout.
			  // the above thing is an error because emptying the buffer
			  // triggers another EPOLLIN
#endif
        rt = RunTime::getInstance();
        rt->requestPool.push_back(req);
        return;
    }
    if (bitflag & EPOLLOUT) {
        Logger::log(LOG_WARNING, "Todo pollout on Client");
    }
    return;
}

// TODO: Implement this header parsing
AHttpRequest *Client::parseHeader(void) {
    const int     numb = (rand() + 1) % 100;
    AHttpRequest *req;

    Logger::log(LOG_WARNING, "Mocked parseHeader on Client");
    switch (numb) {
    case 1 ... 33:
        req = new (std::nothrow) RegularRequest(this);
        break;
    case 34 ... 66:
        req = new (std::nothrow) MultipartRequest(this);
        break;
    case 67 ... 100:
        req = new (std::nothrow) CgiRequest(this);
        break;
    }
    return (req);
}

struct epoll_event Client::getEpollEventStruct(void) const throw() {
    return ((const epoll_event){
        .events = EPOLLIN | EPOLLOUT | EPOLLET,
        .data = {.ptr = (void *)this},
    });
}
