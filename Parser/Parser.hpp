#pragma once
#include <map>
#ifndef PARSER

#include "../runTime/Logger.hpp"
#include "./tokenizer/Lexer.hpp"
#include "ast/Ast.hpp"
#include "ast/LocationStatement.hpp"
#include "ast/ServerStatement.hpp"
#include <vector>

#include "./Parser.hpp"
#include "ast/IAst.hpp"
#include "ast/ListenStatement.hpp"
#include "tokenizer/Token.hpp"

#include <string>

class Parser {
  public:
    Parser(Lexer *);
    ~Parser();
    Parser(std::string);
    ServerConfig            *ParseConfig();
    void                     check_parser_errors();
    std::vector<std::string> errors;

  private:
    Lexer *lexer;
    Token *current_token;
    Token *peek_token;

    bool current_token_is(token_type);
    bool peek_token_is(token_type);
    bool expect_peek(token_type);
    void peek_error(token_type);

    void next_token();
    // TOP LEVEL
    IStatement      *parse_statement();
    ServerStatement *parse_server_statement();
    void             parse_inside_server_block_statment(ServerStatement *);

    // LEVEL 1
    ListenStatement *parse_listen_statement();
    std::string      parse_root_statement();
    std::string      parse_server_name_statement();
    std::string      parse_client_max_body_size_statement();
    std::string      parse_client_max_header_size_statement();
    void parse_error_page_statement(std::map<std::string, std::string> &);

    // LEVEL 2
    LocationStatement *parse_location_statement();
    void        parse_inside_location_block_statement(LocationStatement *);
    std::string parse_return_statment();
    bool        parse_autoindex_statement();
    void        parse_allow_methods_statement(std::vector<std::string> &);
    void        parse_cgi_extensions_statment(std::vector<std::string> &);
    bool        isValidMethod(token_type);
    /* std::vector<std::string> parse_allow_methods_statment(); */
    /* std::vector<std::string> parse_cgi_extensions_statment(); */
};

#endif // !PARSER
