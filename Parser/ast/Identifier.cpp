#include "Identifier.hpp"

void        Identifier::statement_node() {}
std::string Identifier::token_literal() { return this->token.get_literal(); }
