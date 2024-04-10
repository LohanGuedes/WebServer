#pragma once
#ifndef AST_HPP
#define AST_HPP

#include "./IAst.hpp"

/* vaitomarnocusaiago n aguento mais codar cpp */
class ListenStatement {
public:
  Token tok;
  IExpression value;
};

class ServerConfig {
private:
  std::vector<IStatement> statements;

public:
  std::string token_literal();
};

#endif // !AST_HPP
