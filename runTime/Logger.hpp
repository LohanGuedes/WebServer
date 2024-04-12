#ifndef LOGGER_HPP
#define LOGGER_HPP
#pragma once
#include <ctime>
#include <iostream>

#define COLOR_RED "\u001b[31m"
#define COLOR_YELLOW "\u001b[33m"
#define COLOR_END "\u001b[0m"

enum LOG_LEVEL { LOG_INFO, LOG_ERROR, LOG_WARNING };

class Logger {
  public:
    Logger(void);
    ~Logger(void);
    static void log(LOG_LEVEL, const std::string) throw();
};

#endif // ! LOGGER_HPP
