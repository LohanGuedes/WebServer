#include "Token.hpp"

Token::Token(token_type t_type, std::string literal)
    : _literal(literal), _type(t_type) {
  return;
}

Token::Token() { return; }

Token::~Token() {}

void Token::set_literal(std::string literal) { this->_literal = literal; }

std::string Token::get_literal() { return this->_literal; }
token_type Token::get_type() { return this->_type; }

void Token::set_type(token_type t_type) { this->_type = t_type; }

bool Token::is_letter(char ch) {
  return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_' ||
         ch == '/';
}

token_type Token::get_identifier(std::string ident) {
  /* Keyword check -> C++98 static hashmaps can be a hassle */
  if (ident == "server") {
    return token_type::server;
  }
  if (ident == "listen") {
    return token_type::listen;
  }
  if (ident == "server_name") {
    return token_type::server_name;
  }
  if (ident == "location") {
    return token_type::location;
  }
  if (ident == "root") {
    return token_type::root;
  }
  if (ident == "error_page") {
    return token_type::error_page;
  }
  if (ident == "auto_index") {
    return token_type::auto_index;
  }
  if (ident == "client_max_body_size") {
    return token_type::client_max_body_size;
  }
  if (ident == "cgi_extension") {
    return token_type::cgi_extension;
  }
  if (ident == "return") {
    return token_type::ret;
  }
  if (ident == "allow_methods") {
    return token_type::allow_methods;
  }

  // User defined identifier
  return token_type::identifier;
}
