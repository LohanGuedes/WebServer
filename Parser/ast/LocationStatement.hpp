#pragma once
#ifndef LOCATIONSTATMENT

#include "IAst.hpp"
#include "Identifier.hpp"
#include <vector>

class LocationStatement : public IStatement {
  public:
    LocationStatement(void);
    Token      token;
    Identifier path;

    // Inside brackets information
    std::string              path_str;
    std::vector<std::string> allowed_methods;
    std::vector<std::string> cgi_extensions;
    std::string              return_location;
    bool                     autoindex;

    // Implementation for IStatement & INode
    virtual std::string token_literal();
    void                statement_node();

  private:
};

#endif // !LOCATIONSTATMENT
