#ifndef RUNTIME_HPP
#define RUNTIME_HPP
#include "ISocket.hpp"
#include <iostream>
#include <vector>

class RunTime {
public:
  ~RunTime(void);
  static RunTime *getInstance(void);
  static void deleteInstance(void);
  int epoll_fd;

private:
  RunTime(void);
  static RunTime *_instance;
};

#endif // RunTime
