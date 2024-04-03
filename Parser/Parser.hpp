#pragma once
#ifndef PARSER

#include "./tokenizer/Lexer.hpp"
#include "ast/Ast.hpp"

#include <string>

class Parser {
public:
  Parser(Lexer *);
  Parser(std::string);
  ServerConfig ParseConfig();

private:
  Lexer *lexer;
  void next_token();

  Token current_token;
  Token peer_token;
};

#endif // !PARSER
