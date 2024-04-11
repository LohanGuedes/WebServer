#pragma once
#ifndef AST_HPP
#define AST_HPP

#include "./IAst.hpp"

/* ServerConfig class references the whole configure found inside
 * the "nginx.conf" file (a small subset anyways).
 */
class ServerConfig {
  public:
    std::vector<IStatement *> statements;
    std::string               token_literal();
    int                       statements_len();
};

#endif // !AST_HPP
