#pragma once
#ifndef IAST_HPP
#define IAST_HPP

#include "../tokenizer/Token.hpp"

#include <string>
#include <vector>

class INode {
  public:
    virtual std::string token_literal();
};

class IStatement : public INode {
  public:
    void statement_node();
};

class IExpression : public INode {
  public:
    void expression_node();
};

#endif // !IAST_HPP
