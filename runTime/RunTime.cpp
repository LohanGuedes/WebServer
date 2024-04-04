#include "RunTime.hpp"
#include "APollable.hpp"
#include "Logger.hpp"
#include <sys/epoll.h>

// Static variables have to be initialized like this
RunTime *RunTime::_instance = NULL;

RunTime::RunTime(void) : _epoll_fd(-1) {}

RunTime *RunTime::getInstance(void) throw() {
  if (!RunTime::_instance) {
    Logger::log(LOG_INFO, "RunTime instance created");
    RunTime::_instance = new RunTime();
  }
  return (RunTime::_instance);
}

void RunTime::deleteInstance(void) throw() {
  // Implemente o cleanup das coisas no destrutor pra
  // gente usar RAII igual gente.
  if (!RunTime::_instance) {
    return;
  }
  delete RunTime::getInstance();
}

RunTime::~RunTime(void) { Logger::log(LOG_INFO, "RunTime instance deleted"); }

int RunTime::getEpollFd(void) const throw() { return (this->_epoll_fd); }

bool RunTime::startEpollInstance(void) {
  if (this->_epoll_fd == -1) {
    this->_epoll_fd = epoll_create1(EPOLL_CLOEXEC);
  }
  return (this->_epoll_fd != -1);
}

bool RunTime::addToEpoll(const APollable *newInstance) throw() {
  const struct epoll_event events = newInstance->getEpollEventStruct();

  return (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, *newInstance->fd_ptr,
                    const_cast<struct epoll_event *>(&events)));
}
#if 0
// toda request que chegar vai ser um novo client, a não ser que seja uma
// request em chunk. Essa solução abaixo vai permitir que eu leia em partes
// requests com body muito grande e também requests em chunk.
// this is going to be the event loop for a request reading from the socket.
void read_from_sock() {
  static int status; // ou isso ou uma variável dentro de cada Request object
  int header_size;
  int body_status;

  switch (status) {
  case (0):
    header_size = read_header();
    if (header_size > HEADER_HARD_CEILING) {
      return (ERRO);
    }
    status++;
  case (1):
    body_status = processar_body();
    switch (body_status) {
    case (ERROR):
      return (ERRO);
      break;
    case (FINISHED):
      status++;
      break;
    case (STILL_TO_READ):
      break;
    }
  }
}
#endif
