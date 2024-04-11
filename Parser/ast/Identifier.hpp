#pragma once
#ifndef LEX_IDENTIFIER

#include "../tokenizer/Token.hpp"
#include <string>

class Identifier {
  public:
    Token       token; // See token_type::identifier
    std::string value;
    std::string token_literal();
    void        statement_node();
};

#endif // !LEX_IDENTIFIER
