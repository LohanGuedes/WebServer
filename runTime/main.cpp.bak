#include "Listener.hpp"
#include "Logger.hpp"
#include "RunTime.hpp"
#include <sys/epoll.h>
#include <vector>

int main(void) {
    RunTime        *runTime = RunTime::getInstance(); // initialize RunTime
    const Listener *listenv6 = new Listener("127.0.0.1", "8080");
    const Listener *listenv4 = new Listener("0:0:0:0:0:0:0:1", "8081");
    int             checkType;

    runTime->addListener(listenv4);
    runTime->addListener(listenv6);
    runTime->createEpollInstance();
    runTime->startListeners();
    runTime->addListenersToEpoll();
    while (true) {
        checkType = (runTime->requestPool.size() == 0) ? BLOCKING_CHECK
                                                       : NONBLOCKING_CHECK;
        runTime->checkEpoll(checkType);
        runTime->processRequests();
    }

    RunTime::deleteInstance(); // delete RunTime
    return (0);
}
#if 0
int main(void) {
  Listener listener("127.0.0.1", 8080);
  std::vector<struct epoll_event> ev_vec(1);
  int instance;
  int size;
  ISocket *sock_ptr;

  listener.listen();
  instance = epoll_create1(EPOLL_CLOEXEC);
  epoll_ctl(instance, EPOLL_CTL_ADD, listener.getFd(),
            const_cast<struct epoll_event *>(&listener.epoll_conf));
  ev_vec.resize(ev_vec.size() + 1);
  while (true) {
    size = epoll_wait(instance, &ev_vec.front(), ev_vec.size(), -1);
    if (size < 0) {
      std::cerr << "fudeu" << std::endl;
      return (EXIT_FAILURE);
    }
    for (std::vector<struct epoll_event>::size_type i = 0;
         i < static_cast<std::vector<struct epoll_event>::size_type>(size) &&
         i < ev_vec.size();
         i++) {
      sock_ptr = reinterpret_cast<ISocket *>(ev_vec[i].data.ptr);
      sock_ptr->handlePoll(ev_vec[i].events);
    }
  }
}
#endif
