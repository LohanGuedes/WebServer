#include "./Ast.hpp"

std::string ServerConfig::token_literal() {
    if (this->statements.size()) {
        return this->statements[0]->token_literal();
    } else {
        return "";
    }
}

int ServerConfig::statements_len() { return this->statements.size(); }
