#include "RunTime.hpp"
#include "APollable.hpp"
#include "Listener.hpp"
#include "Logger.hpp"
#include <new>
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

inline int RunTime::getEpollInstance() const throw() {
    return (this->_epollInstance);
}

// initialization //

int RunTime::addListener(std::string const &host,
                         std::string const &port) throw() {
    unsigned long hash = Listener::hashStr(host + port);
    Listener     *newListener;

    for (size_t i = 0; i < this->listenerPool.size(); i++) {
        if (this->listenerPool[i]->hostPortHash == hash) {
            return (1);
        }
    }
    newListener = new (std::nothrow) Listener(host, port);
    if (newListener == NULL) {
        return (-1);
    }
    this->listenerPool.push_back(newListener);
    return (0);
}

int RunTime::addListener(Listener const *newListener) throw() {
    if (!newListener) {
        return (-1);
    }
    for (size_t i = 0; i < this->listenerPool.size(); ++i) {
        if (this->listenerPool[i]->hostPortHash == newListener->hostPortHash) {
            return (1);
        }
    }
    this->listenerPool.push_back(newListener);
    return (0);
}

Listener const *RunTime::getListener(std::string const &host,
                                     std::string const &port) const throw() {
    const unsigned int hash = Listener::hashStr(host + port);

    for (size_t i = 0; i < this->listenerPool.size(); i++) {
        if (this->listenerPool[i]->hostPortHash == hash) {
            return (this->listenerPool[i]);
        }
    }
    return (NULL);
}

Listener const *RunTime::getListener(const unsigned int hash) const throw() {
    for (size_t i = 0; i < this->listenerPool.size(); i++) {
        if (this->listenerPool[i]->hostPortHash == hash) {
            return (this->listenerPool[i]);
        }
    }
    return (NULL);
}

bool RunTime::createEpollInstance(void) throw() {
    if (this->_epollInstance == -1) {
        this->_epollInstance = epoll_create1(EPOLL_CLOEXEC);
    }
    return (this->_epollInstance != -1);
}

bool RunTime::startListeners(void) const {
    std::stringstream ss;

    const std::vector<const Listener *>::size_type size =
        this->listenerPool.size();

    for (std::vector<const Listener *>::size_type i = 0; i < size; i++) {
        this->listenerPool[i]->listen();
        ss.clear();
        ss << "Starting Listener host [" << this->listenerPool[i]->host
           << "] and port [" << this->listenerPool[i]->port << "]";
        Logger::log(LOG_INFO, ss.str());
    }
    return (true);
}

bool RunTime::addListenersToEpoll(void) throw() {
    const std::vector<const Listener *>::size_type size =
        this->listenerPool.size();

    for (std::vector<const Listener *>::size_type i = 0; i < size; i++) {
        this->addToEpoll(this->listenerPool[i]);
    }

    return (true);
}

// event loop //

bool RunTime::addToEpoll(const APollable *newInstance) throw() {
    const struct epoll_event events = newInstance->getEpollEventStruct();

    return (epoll_ctl(this->_epollInstance, EPOLL_CTL_ADD, *newInstance->fd_ptr,
                      const_cast<struct epoll_event *>(&events)) == 0 &&
            this->epollCount++);
}

bool RunTime::checkEpoll(int checkType) const throw() {
    Logger::log(LOG_INFO, "Entrou na checkEpoll");
    std::vector<struct epoll_event>            events(this->epollCount);
    std::vector<struct epoll_event>::size_type i = 0;
    APollable                                 *extractedData;
    std::stringstream                          ss;

    ss << "O valor de i é " << checkType;
    Logger::log(LOG_INFO, ss.str());
    const int loop_ceiling = epoll_wait(this->_epollInstance, &events.at(0),
                                        this->epollCount, checkType);
    if (loop_ceiling < 0) {
        return (false);
    }
    Logger::log(LOG_INFO, "Passou do epoll_wait");
    while (i < (std::vector<struct epoll_event>::size_type)loop_ceiling) {
        //              c++ moment KEKW
        extractedData = reinterpret_cast<APollable *>(events[i].data.ptr);
        extractedData->handlePoll(events[i].events);
        i++;
    }
    return (true);
}

bool RunTime::processRequests(void) {
    std::vector<int>::size_type size = this->requestPool.size();
    for (std::vector<int>::size_type i = 0; i < size; i++) {
        std::cout << "request " << i << "sendo processada" << std::endl;
    }
    return (true);
}

// cleanup
bool RunTime::closeListeners(void) throw() {
    std::stringstream                              ss;
    const std::vector<const Listener *>::size_type size =
        this->listenerPool.size();

    for (std::vector<const Listener *>::size_type i = 0; i < size; i++) {
        ss.clear();
        ss << "Stopping Listener host [" << this->listenerPool[i]->host
           << "] and port [" << this->listenerPool[i]->port << "]";
        Logger::log(LOG_INFO, ss.str());
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
