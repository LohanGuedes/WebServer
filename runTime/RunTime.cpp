#include "RunTime.hpp"

// Static variables have to be initialized like this
RunTime *RunTime::_instance = NULL;

RunTime::RunTime(void) {}

const RunTime *RunTime::getInstance(void) {
  if (!RunTime::_instance) {
    RunTime::_instance = new RunTime();
  }
  return (RunTime::_instance);
}

RunTime::~RunTime(void) {}
