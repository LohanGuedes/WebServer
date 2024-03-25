#pragma once

#include "./token.hpp"
#include <string>

class Lexer {
public:
  Lexer(const std::string);
  ~Lexer();
  void read_char(void);
  Token *next_token();
  std::string read_identifier();

private:
  std::string _input;

  /* current position in input (points to current char) */
  size_t _position;

  /* current reading position in input (points to the next char) */
  size_t _read_position;

  /* current char under examination */
  char _byte;
};
