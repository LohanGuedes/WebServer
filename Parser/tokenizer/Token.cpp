#include "Token.hpp"

const char *Token::token_names[] = {
    "COMMA ','",
    "SEMICOLON ';'",
    "LEFT-BRACKET '{'",
    "RIGHT-BRACKET '}'",

    "IDENTIFIER",

    "keyword 'server'",
    "keyword 'listen'",
    "keyword 'server_name'",
    "keyword 'location'",
    "keyword 'root'",
    "keyword 'error_page'",
    "keyword 'auto_index'",
    "keyword 'client_max_body_size'",
    "keyword 'cgi_extension'",
    "keyword 'allow_methods'",
    "keyword 'return'",

    "HTTP method 'GET'",
    "HTTP method 'POST'",
    "HTTP method 'DELTE'",
    "end of file",
    "ILLEGALTOKEN",
};

Token::Token(token_type t_type, std::string literal)
    : _literal(literal), _type(t_type) {
    return;
}

Token::Token() { return; }

Token::~Token() {}

void Token::set_literal(std::string literal) { this->_literal = literal; }

std::string Token::get_literal() { return this->_literal; }
token_type  Token::get_type() { return this->_type; }

void Token::set_type(token_type t_type) { this->_type = t_type; }

bool Token::is_letter(char ch) {
    return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_' ||
           ch == '/' || ch == ':' || (ch >= '0' && ch <= '9');
}

token_type Token::get_identifier(std::string ident) {
    /* Keyword check -> C++98 static hashmaps can be a hassle */
    if (ident == "server") {
        return SERVER;
    }
    if (ident == "listen") {
        return LISTEN;
    }
    if (ident == "server_name") {
        return SERVER_NAME;
    }
    if (ident == "location") {
        return LOCATION;
    }
    if (ident == "root") {
        return ROOT;
    }
    if (ident == "error_page") {
        return ERROR_PAGE;
    }
    if (ident == "auto_index") {
        return AUTO_INDEX;
    }
    if (ident == "client_max_body_size") {
        return CLIENT_MAX_BODY_SIZE;
    }
    if (ident == "cgi_extension") {
        return CGI_EXTENSION;
    }
    if (ident == "return") {
        return RET;
    }
    if (ident == "allow_methods") {
        return ALLOW_METHODS;
    }

    // User defined identifier
    return IDENTIFIER;
}
