#pragma once
#ifndef AST_HPP
#define AST_HPP

#include "./IAst.hpp"

class ServerStatement;

/* ServerConfig class references the whole configure found inside
 * the "nginx.conf" file (a small subset anyways).
 */
class ServerConfig {
  public:
    std::vector<ServerStatement *> statements;
};

#endif // !AST_HPP
