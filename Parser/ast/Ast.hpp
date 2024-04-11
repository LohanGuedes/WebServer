#pragma once
#ifndef AST_HPP
#define AST_HPP

#include "./IAst.hpp"

/* ServerConfig class references the whole configure found inside
 * the "nginx.conf" file (a small subset anyways).
 */
class ServerConfig {
  private:
    std::vector<IStatement> statements;

  public:
    std::string token_literal();
};

#endif // !AST_HPP
