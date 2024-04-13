#include "./Parser.hpp"
#include "ast/Identifier.hpp"
#include "ast/ListenStatement.hpp"
#include "tokenizer/Token.hpp"

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
    }
    return NULL;
}

ListenStatement *Parser::parse_listen_statement() {
    ListenStatement *stmt = new ListenStatement();
    stmt->token.set_type(this->current_token->get_type());
    stmt->token.set_literal(this->current_token->get_literal());

    if (!this->expectPeek(IDENTIFIER)) {
        return NULL;
    }

    stmt->host_port.token = this->current_token;
    stmt->host_port.value = this->current_token->get_literal();

    // TODO: Skipping expressions until token_type::SEMICOLON is found
    while (!this->current_token_is(SEMICOLON)) {
        this->next_token();
    }

    return stmt;
}

bool Parser::current_token_is(int t_enum) {
    return this->current_token->get_type() == t_enum;
}

bool Parser::peek_token_is(int t_enum) {
    return this->peek_token->get_type() == t_enum;
}

bool Parser::expectPeek(int t_enum) {
    if (this->peek_token_is(t_enum)) {
        this->next_token();
        return true;
    }

    return false;
}
