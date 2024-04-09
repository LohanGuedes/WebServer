#include "Client.hpp"
#include "APollable.hpp"
#include <sys/epoll.h>

Client::Client(int fd) : APollable(this->getEpollEventStruct()) {
  this->_fd = fd;
}

Client::~Client(void) {}

void Client::handlePoll(epoll_event_bitflag bitflag) {
  (void)bitflag;
  if (bitflag & EPOLLIN) {
    Logger::log(LOG_WARNING, "todo: handlePoll for Client");
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
