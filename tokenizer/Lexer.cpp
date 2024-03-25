#include "Lexer.hpp"

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
  switch (this->_byte) {
  case ';':
    return new Token(token_type::semicolon, std::string(1, this->_byte));
    break;
  case '{':
    return new Token(token_type::rbracket, std::string(1, this->_byte));
    break;
  case '}':
    return new Token(token_type::lbracket, std::string(1, this->_byte));
    break;
  case ',':
    return new Token(token_type::comma, std::string(1, this->_byte));
    break;
  case '\0':
    return new Token(token_type::eof, "");
    break;
  default:
    /* the following if might be wrong... Write a custom isLetter or isAlpha
     * check for: ":,_-" */
    if (std::isalpha(this->_byte)) {
      std::string ident = this->read_identifier();
      return new Token(Token::get_identifier(ident), ident);
    } else {
      return new Token(token_type::illegal, std::string(1, this->_byte));
    }
  }

  // TODO: THIS IS WRONG!!!!
  /* this->read_char(); */
  return new Token(token_type::illegal, "");
}

std::string Lexer::read_identifier() {
  size_t start = this->_position;

  while (std::isalpha(this->_byte)) {
    this->read_char();
  }

  return this->_input.substr(start, this->_position);
}
