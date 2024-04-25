#pragma once
#ifndef LISTEN_STATEMENT

#include "../tokenizer/Token.hpp"
#include "Ast.hpp"
#include "IAst.hpp"
#include "Identifier.hpp"
#include <string>

/* ListenStatement references the Listen rule inside a server{}
 * directive, from where a Listener will be created.
 *
 * Example of a ListenStatement/directive:
 * Listen <string(<HOST>:<PORT>)>;
 *
 * ListenStatement {
 *  .token = token_type::listen
 *  .host  = std::string("<HOST/URL>")
 *  .port  = std::string("<PORT>")
 * }
 */
class ListenStatement : public IStatement {
  public:
    Token token;
    // TODO: Split host_port
    Identifier host_port; // <URL>:<PORT>
    // Implementation for IStatement & INode
    virtual std::string token_literal();
    void                statement_node();
    ListenStatement(void);

  private:
};

#endif // !LISTEN_STATEMENT
