#ifndef RUNTIME_HPP
#define RUNTIME_HPP
#include <iostream>

class RunTime {
public:
  ~RunTime(void);
  static const RunTime *getInstance(void);
  std::string str;

private:
  RunTime(void);
  static RunTime *_instance;
};

#endif // RunTime
