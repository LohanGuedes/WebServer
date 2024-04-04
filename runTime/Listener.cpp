#include "Listener.hpp"
#include "APollable.hpp"
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>

Listener::Listener(std::string address, int port)
    : APollable(Listener::getEpollEventStruct()), address(address), port(port) {

  const int val = 1;
  const struct sockaddr_in conf = {
      .sin_family = AF_INET,
      .sin_port = ntohs(port),
      .sin_addr = {.s_addr = inet_addr(address.c_str())},
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

Listener::~Listener(void) { close(this->_fd); }

void Listener::handlePoll(epoll_event_bitflag bitflag) {
  std::cout << "Epoll instance returned listener" << std::endl;
  if (bitflag & EPOLLIN) {
    std::cout << "EPOLLIN hit" << std::endl;
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

struct epoll_event Listener::getEpollEventStruct(void) const throw() {
  return ((struct epoll_event){.events = EPOLLIN | EPOLLET,
                               .data = {.ptr = (void *)this}});
}
