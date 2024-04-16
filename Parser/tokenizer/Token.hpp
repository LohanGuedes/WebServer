#pragma once

#include <string>
#include <vector>

enum token_type {
    // DELIMITERS
    COMMA,
    SEMICOLON,
    LBRACKET,
    RBRACKET,

    // NOTE: USER DEFINED IDENTIFIER
    IDENTIFIER,

    // KEYWORDS
    SERVER,
    LISTEN,
    SERVER_NAME,
    LOCATION,
    ROOT,
    ERROR_PAGE,
    AUTO_INDEX,
    CLIENT_MAX_BODY_SIZE,
    CGI_EXTENSION,
    ALLOW_METHODS,
    RET, // RETURN KEYWORD

    // HTTP ALLOWED METHODS
    M_GET,
    M_POST,
    M_DELETE,

    T_EOF,
    ILLEGAL
};

class Token {
  public:
    static const char *token_names[];
    void               set_literal(std::string);
    void               set_type(token_type);

    token_type  get_type();
    std::string get_literal();

    Token(token_type, std::string);
    Token();
    ~Token();

    static bool is_letter(char);

    static token_type get_identifier(std::string);

  private:
    std::string _literal;
    token_type  _type;
};
