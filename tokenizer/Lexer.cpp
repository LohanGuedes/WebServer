#include "Lexer.hpp"
#include "Token.hpp"

#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string file_content)
    : _input(file_content), _position(0), _read_position(0),
      _byte(file_content[0]) {

  this->read_char(); // initialize this->_read_position
}

Lexer::~Lexer() { return; };

void Lexer::read_char(void) {
  if (this->_read_position >= this->_input.length()) {
    this->_byte = 0;
  } else {
    this->_byte = this->_input[this->_read_position];
  }

  this->_position = this->_read_position;
  this->_read_position++;
}

Token *Lexer::next_token() {
  Token *tok = new Token();

  this->consume_whitespace();

  switch (this->_byte) {
  case ';':
    tok->set_type(token_type::semicolon);
    tok->set_literal(std::string(1, this->_byte));
    break;
  case '{':
    tok->set_type(token_type::lbracket);
    tok->set_literal(std::string(1, this->_byte));
    break;
  case '}':
    tok->set_type(token_type::rbracket);
    tok->set_literal(std::string(1, this->_byte));
    break;
  case ',':
    tok->set_type(token_type::comma);
    tok->set_literal(std::string(1, this->_byte));
    break;
  case '\0':
    tok->set_type(token_type::eof);
    tok->set_literal(std::string(1, this->_byte));
    break;
  default:
    if (Token::is_letter(this->_byte)) {
      tok->set_literal(this->read_identifier());
      tok->set_type(Token::get_identifier(tok->get_literal()));
    } else {
      return new Token(token_type::illegal, std::string(1, this->_byte));
    }
  }

  this->read_char();
  return tok;
}

std::string Lexer::read_identifier() {
  size_t start = this->_position;

  while (Token::is_letter(this->_byte)) {
    this->read_char();
  }

  return this->_input.substr(start, this->_position - start);
}

void Lexer::consume_whitespace() {
  while (this->_byte == ' ' || this->_byte == '\t' || this->_byte == '\n' ||
         this->_byte == '\r') {
    this->read_char();
  }
}
