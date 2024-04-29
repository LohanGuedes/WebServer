#pragma once
#ifndef SERVERSTATEMENT

#include "IAst.hpp"
#include "Identifier.hpp"
#include "ListenStatement.hpp"
#include "LocationStatement.hpp"
#include "error_page_statement.hpp"
#include <map>
#include <string>
#include <vector>

class ServerStatement : public IStatement {
  public:
    ServerStatement(void);
    Token      token;
    Identifier ident;

    std::string                            server_name;
    std::vector<const LocationStatement *> locationStatements;
    std::vector<const ListenStatement *>   listeners;
    std::string                            root;
    std::string                            client_max_body_size;
    std::string                            client_max_header_size;
    std::map<std::string, std::string>     error_page_statements;

    // Implementation for IStatement & INode |> Prolly not necessary
    virtual std::string token_literal();
    void                statement_node();

  private:
};

#endif // !SERVERSTATEMENT
