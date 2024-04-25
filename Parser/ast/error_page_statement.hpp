#pragma once
#ifndef ERROR_PAGE_STATEMENT

#include "IAst.hpp"
#include "Identifier.hpp"
#include <vector>

class ErrorPageStatement : public IStatement {
  public:
    ErrorPageStatement(void);
    Token      token;
    Identifier ident;

    int         status_code;
    std::string content_path;

    // Implementation for IStatement & INode |> Prolly not necessary
    virtual std::string token_literal();
    void                statement_node();

  private:
};

#endif // !ERROR_PAGE_STATEMENT
