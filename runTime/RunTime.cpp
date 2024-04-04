#include "RunTime.hpp"
#include "APollable.hpp"
#include "Listener.hpp"
#include "Logger.hpp"
#include <sstream>
#include <sys/epoll.h>
#include <vector>

// Static variables have to be initialized like this
RunTime *RunTime::_instance = NULL;

RunTime::RunTime(void) : epollCount(0), _epollInstance(-1) {}

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

RunTime::~RunTime(void) {
  this->closeListeners();
  Logger::log(LOG_INFO, "RunTime instance deleted");
}

bool RunTime::startEpollInstance(void) throw() {
  if (this->_epollInstance == -1) {
    this->_epollInstance = epoll_create1(EPOLL_CLOEXEC);
  }
  return (this->_epollInstance != -1);
}

bool RunTime::addToEpoll(const APollable *newInstance) throw() {
  const struct epoll_event events = newInstance->getEpollEventStruct();

  return (epoll_ctl(this->_epollInstance, EPOLL_CTL_ADD, *newInstance->fd_ptr,
                    const_cast<struct epoll_event *>(&events)) &&
          this->epollCount++);
}

bool RunTime::checkEpoll(void) const throw() {
  Logger::log(LOG_INFO, "Entrou na checkEpoll");
  std::vector<struct epoll_event> events(this->epollCount);
  std::vector<struct epoll_event>::size_type i = 0;
  APollable *extractedData;

  const int loop_ceiling =
      epoll_wait(this->_epollInstance, &events.at(0), this->epollCount, 0);
  if (loop_ceiling < 0) {
    return (false);
  }
  Logger::log(LOG_INFO, "Passou do epoll_wait");
  while (i < (std::vector<struct epoll_event>::size_type)loop_ceiling) {
    extractedData = reinterpret_cast<APollable *>(events[i].data.ptr);
    extractedData->handlePoll(events[i].events);
    i++;
  }
  return (true);
}

inline int RunTime::getEpollInstance() const throw() {
  return (this->_epollInstance);
}

bool RunTime::startListeners(void) const {
  std::stringstream ss;

  const std::vector<const Listener *>::size_type size =
      this->listenerPool.size();

  for (std::vector<const Listener *>::size_type i = 0; i < size; i++) {
    this->listenerPool[i]->listen();
    ss.clear();
    ss << "Listener started for host [" << this->listenerPool[i]->address
       << "] and port [" << this->listenerPool[i]->port << "]";
    Logger::log(LOG_INFO, ss.str());
  }
  return (true);
}

//
bool RunTime::closeListeners(void) throw() {
  std::stringstream ss;
  const std::vector<const Listener *>::size_type size =
      this->listenerPool.size();

  for (std::vector<const Listener *>::size_type i = 0; i < size; i++) {
    ss.clear();
    ss << "Stopping Listener host [" << this->listenerPool[i]->address
       << "] and port [" << this->listenerPool[i]->port << "]";
    Logger::log(LOG_INFO, ss.str());
    close(*this->listenerPool[i]->fd_ptr);
    delete this->listenerPool[i];
  }

  return (true);
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
