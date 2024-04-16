#pragma once
#include <vector>
#ifndef PARSER

#include "./tokenizer/Lexer.hpp"
#include "ast/Ast.hpp"

#include "./Parser.hpp"
#include "ast/IAst.hpp"
#include "ast/ListenStatement.hpp"
#include "tokenizer/Token.hpp"

#include <string>

class Parser {
  public:
    Parser(Lexer *);
    Parser(std::string);
    ServerConfig            *ParseConfig();
    std::vector<std::string> errors;

  private:
    Lexer           *lexer;
    void             next_token();
    IStatement      *parse_statement();
    ListenStatement *parse_listen_statement();

    bool current_token_is(token_type);
    bool peek_token_is(token_type);
    bool expect_peek(token_type);
    void peek_error(token_type);

    // not const. but hope theres not side-effect weretogether
    Token *current_token;
    Token *peek_token;
};

#endif // !PARSER
