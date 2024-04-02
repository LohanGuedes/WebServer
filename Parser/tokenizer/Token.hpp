#pragma once

#include <string>

enum class token_type {

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
  m_delete,

  eof,
  illegal,
};

class Token {
public:
  void set_literal(std::string);
  void set_type(token_type);

  token_type get_type();
  std::string get_literal();

  Token(token_type, std::string);
  Token();
  ~Token();

  static bool is_letter(char);

  static token_type get_identifier(std::string);

private:
  std::string _literal;
  token_type _type;
};
