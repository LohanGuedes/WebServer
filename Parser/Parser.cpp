#include "./Parser.hpp"
#include "ast/Identifier.hpp"
#include "ast/ListenStatement.hpp"
#include "tokenizer/Token.hpp"
#include <sstream>
#include <vector>

Parser::Parser(Lexer *lexer) {
    this->lexer = lexer;

    // Lohan burro burro burro
    this->next_token();
    this->next_token();
}

Parser::Parser(std::string file_path) {
    // TODO: Implement this using open and whynot
    return; // :)
}

ServerConfig *Parser::ParseConfig() {
    ServerConfig *serverConf = new ServerConfig();
    while (this->current_token->get_type() != T_EOF) {
        IStatement *stmt = this->parse_statement();
        if (stmt != NULL) {
            serverConf->statements.push_back(stmt);
        }
        this->next_token();
    }

    return serverConf;
}

void Parser::next_token() {
    this->current_token = this->peek_token;
    this->peek_token = this->lexer->next_token();
    return;
}

IStatement *Parser::parse_statement() {
    switch (this->current_token->get_type()) {
    case LISTEN:
        return this->parse_listen_statement();
    case LOCATION:
        return NULL;
    }
    return NULL;
}

ListenStatement *Parser::parse_listen_statement() {
    ListenStatement *stmt = new ListenStatement();
    stmt->token.set_type(this->current_token->get_type());
    stmt->token.set_literal(this->current_token->get_literal());

    if (!this->expect_peek(IDENTIFIER)) {
        return NULL;
    }

    stmt->host_port.token = this->current_token;
    stmt->host_port.value = this->current_token->get_literal();

    if (!this->expect_peek(SEMICOLON)) {
        return NULL;
    }

    return stmt;
}

bool Parser::current_token_is(token_type type) {
    return this->current_token->get_type() == type;
}

bool Parser::peek_token_is(token_type type) {
    return this->peek_token->get_type() == type;
}

bool Parser::expect_peek(token_type type) {
    if (this->peek_token_is(type)) {
        this->next_token();
        return true;
    }

    this->peek_error(type);
    return false;
}

void Parser::peek_error(token_type type) {
    std::stringstream msg;

    msg << "Expected token with type: " << Token::token_names[type]
        << " got: " << Token::token_names[this->peek_token->get_type()]
        << std::endl;

    this->errors.push_back(msg.str());
}
