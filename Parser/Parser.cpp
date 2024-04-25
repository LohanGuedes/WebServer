#include "./Parser.hpp"
#include "ast/IAst.hpp"
#include "ast/Identifier.hpp"
#include "ast/ListenStatement.hpp"
#include "ast/LocationStatement.hpp"
#include "ast/ServerStatement.hpp"
#include "tokenizer/Token.hpp"
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

Parser::Parser(Lexer *lexer) {
    this->lexer = lexer;

    // Initialize both token-pointers
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
    /* case LISTEN: */
    /*     return this->parse_listen_statement(); */
    case SERVER:
        return this->parse_server_statement();
    default:
        Logger::log(LOG_ERROR,
                    "unexpected token during outer server-block parsing.");
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

void Parser::check_parser_errors() {
    if (this->errors.size() == 0) {
        return;
    }

    Logger::log(LOG_ERROR, "Errors during parsing config...");
    for (int i = 0; i < this->errors.size(); ++i) {
        Logger::log(LOG_ERROR, this->errors[i]);
    }
}

std::string Parser::parse_root_statement() {
    std::string root_path;
    if (!this->expect_peek(IDENTIFIER)) {
        return NULL;
    }
    root_path = this->current_token->get_literal();

    if (!this->expect_peek(SEMICOLON)) {
        return NULL;
    }

    return root_path;
}

std::string Parser::parse_server_name_statement() {
    std::string server_name;
    if (!this->expect_peek(IDENTIFIER)) {
        return NULL;
    }
    server_name = this->current_token->get_literal();

    if (!this->expect_peek(SEMICOLON)) {
        return NULL;
    }

    return server_name;
}

std::string Parser::parse_client_max_body_size_statement() {
    std::string max_body_size_str;
    if (!this->expect_peek(IDENTIFIER)) {
        return NULL;
    }
    max_body_size_str = this->current_token->get_literal();

    if (!this->expect_peek(SEMICOLON)) {
        return NULL;
    }

    return max_body_size_str;
}

std::string Parser::parse_client_max_header_size_statement() {
    std::string max_header_size_str;
    if (!this->expect_peek(IDENTIFIER)) {
        return NULL;
    }
    max_header_size_str = this->current_token->get_literal();

    if (!this->expect_peek(SEMICOLON)) {
        return NULL;
    }

    return max_header_size_str;
}

void Parser::parse_error_page_statement(
    std::map<std::string, std::string> &map) {
    if (!this->expect_peek(IDENTIFIER)) {
        return;
    }

    std::string http_code = this->current_token->get_literal();

    if (!this->expect_peek(IDENTIFIER)) {
        return;
    }

    std::string resource = this->current_token->get_literal();

    if (!this->expect_peek(SEMICOLON)) {
        return;
    }

    map[http_code] = resource;
    return;
}

void Parser::parse_inside_server_block_statment(ServerStatement *stmt) {
    // NOTE: I know there is a warning. but IDGAF.
    switch (this->current_token->get_type()) {
    case LISTEN:
        // TODO: PLEASE THINK ABOUT A WAY TO MAKE THIS WORK
        // PLEASEEEEEEEEEEEEEEEEE this is a problem considering the state during
        // the creation... go read the runtime api bitch.
        return;
    case SERVER_NAME:
        stmt->server_name = this->parse_server_name_statement();
    case ROOT:
        stmt->root = this->parse_root_statement();
        return;
    case CLIENT_MAX_BODY_SIZE:
        stmt->client_max_body_size =
            this->parse_client_max_body_size_statement();
        return;
    /* case CLIENT_MAX_HEADER_SIZE: */
    /*     stmt->client_max_header_size = */
    /*         this->parse_client_max_header_size_statement(); */
    /*     return; */
    case ERROR_PAGE:
        // NOTE: I hate that is not explicit that this will change the state of
        // the hasmap. but its the way I made it work, and I am happy :)
        this->parse_error_page_statement(stmt->error_page_statements);
        return;
    case LOCATION:
        // Think about it...
        return;
    }
}

ServerStatement *Parser::parse_server_statement() {
    ServerStatement *stmt = new ServerStatement();

    // server
    stmt->token.set_type(this->current_token->get_type());
    stmt->token.set_literal(this->current_token->get_literal());

    // {
    if (!this->expect_peek(LBRACKET)) {
        return NULL;
    }

    // While !char('}')
    /*
     * server {
     *    root ./
     *    location asldkfjadk {
     *    }
     * }
     */
    while (!this->current_token_is(RBRACKET)) {
        this->parse_inside_server_block_statment(stmt);
    }

    return stmt;
}

std::string Parser::parse_return_statment() {
    std::string return_location;
    if (!this->expect_peek(IDENTIFIER)) {
        return NULL;
    }
    return_location = this->current_token->get_literal();

    if (!this->expect_peek(SEMICOLON)) {
        return NULL;
    }

    return return_location;
}

// TODO: review this... might be buggy
bool Parser::parse_autoindex_statement() {
    bool return_location;
    if (!this->expect_peek(IDENTIFIER)) {
        return false;
    }
    return_location = this->current_token->get_literal() == "on" ||
                      this->current_token->get_literal() == "true";

    return this->expect_peek(SEMICOLON);
}

bool Parser::isValidMethod(token_type type) {
    return type == M_GET || type == M_POST || type == M_DELETE;
}

void Parser::parse_allow_methods_statement(std::vector<std::string> &methods) {
    // Jump one
    this->next_token();
    while (!this->current_token_is(SEMICOLON)) {

        if (this->isValidMethod(this->current_token->get_type())) {

            methods.push_back(this->current_token->get_literal());
        } else {
            this->peek_error(this->current_token->get_type());
        }
        this->next_token();
    }

    return;
}

// WARNING: May have a off by one error in the state of the parser. maybe we
// need to CONSUME THE SEMICOLON
void Parser::parse_cgi_extensions_statment(std::vector<std::string> &exts) {
    this->next_token();
    while (!this->current_token_is(SEMICOLON)) {
        if (this->current_token_is(IDENTIFIER)) {
            exts.push_back(this->current_token->get_literal());
        } else {
            // TODO: Create a better function for this...
            this->peek_error(this->current_token->get_type());
        }
        this->next_token();
    }
    // WARNING: is this the right way? this->next_token(); -> Consume the ; here
    // or later on? Idnk
    return;
}

void Parser::parse_inside_location_block_statement(LocationStatement *stmt) {
    switch (this->current_token->get_type()) {
    case RET:
        stmt->return_location = this->parse_return_statment();
        break;
    case ALLOW_METHODS:
        this->parse_allow_methods_statement(stmt->allowed_methods);
        break;
    case AUTO_INDEX:
        stmt->autoindex = this->parse_autoindex_statement();
        break;
    case CGI_EXTENSION:
        break;
    }
}

LocationStatement *Parser::parse_location_statement() {
    LocationStatement *stmt = new LocationStatement();

    stmt->token.set_type(this->current_token->get_type());
    stmt->token.set_literal(this->current_token->get_literal());

    if (!this->expect_peek(IDENTIFIER)) {
        return NULL;
    }

    stmt->path_str = this->current_token->get_literal();
    stmt->path.value = this->current_token->get_literal();

    // Walk 1 token forward and check
    if (!this->expect_peek(LBRACKET)) {
        return NULL;
    }

    // Walk once more but now iterating through the new statments that are only
    // possible inside the current location statements.
    while (!this->current_token_is(RBRACKET)) {
        this->parse_inside_location_block_statement(stmt);
    }

    return stmt;
}
