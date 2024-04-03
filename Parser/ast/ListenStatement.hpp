#pragma once
#include "Identifier.hpp"
#ifndef LISTEN_STATEMENT

#include "../tokenizer/Token.hpp"
#include "Ast.hpp"
#include <string>

class ListenStatement {
public:
  Token token; // see token_type::listen
  Identifier *name;
  /* TODO
   * Expression value; */
  std::string token_literal();
  void statement_node();
};

#endif // !LISTEN_STATEMENT
