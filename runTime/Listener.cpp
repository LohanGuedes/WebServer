#include "Listener.hpp"
#include "ISocket.hpp"
#include <sys/epoll.h>

Listener::Listener(std::string address, int port)
    : ISocket(), epoll_conf(Listener::get_epoll_event(this)) {

  int val = 1;
  struct sockaddr_in conf;

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

  // setup the conf struct to bind
  conf.sin_family = AF_INET;
  conf.sin_addr.s_addr = inet_addr(address.c_str());
  conf.sin_port = ntohs(port);
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

void Listener::handlePollin(epoll_event_bitflag value) { (void)value; }

void Listener::handlePollout(void) {}

int Listener::getFd(void) { return (this->_fd); }

void Listener::listen(void) {
  // start listening
  if (::listen(this->_fd, SOMAXCONN) == -1) {
    throw SocketListenException();
  }
  return;
}

struct epoll_event Listener::get_epoll_event(Listener *addr) {
  struct epoll_event ev;

  ev.events = EPOLLIN | EPOLLET;
  ev.data.ptr = addr;
  return (ev);
}
