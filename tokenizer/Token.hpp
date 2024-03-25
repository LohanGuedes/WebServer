#pragma once

#include <string>

enum class token_type {
  eof,
  illegal,

  // delimiters
  comma,
  semicolon,
  lbracket,
  rbracket,

  // NOTE: user defined identifier
  identifier,

  // keywords
  server,
  listen,
  server_name,
  location,
  root,
  error_page,
  auto_index,
  client_max_body_size,
  cgi_extension,
  allow_methods,
  ret, // return keyword

  // http allowed methods
  m_get,
  m_post,
  m_delete
};

class Token {
public:
  void set_literal(std::string);
  void set_type(token_type);
  Token(token_type, std::string);
  ~Token();
  static token_type get_identifier(std::string);

private:
  std::string _literal;
  token_type _type;
};
