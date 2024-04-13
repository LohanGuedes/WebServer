#include "./ListenStatement.hpp"

void ListenStatement::statement_node() { return; }
ListenStatement::ListenStatement(void) { return; }

std::string ListenStatement::token_literal() {
    return this->token.get_literal();
}
