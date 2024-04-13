#pragma once
#ifndef IAST_HPP
#define IAST_HPP

#include "../tokenizer/Token.hpp"

#include <string>
#include <vector>

class INode {
  public:
    virtual std::string token_literal() = 0;
    virtual ~INode() { return; };
};

class IStatement : public INode {
  public:
    virtual void statement_node() = 0;

    // Implements INode
    virtual std::string token_literal() = 0;
};

class IExpression : public INode {
  public:
    virtual void expression_node() = 0;

    // Implements INode
    virtual std::string token_literal() = 0;
};

#endif // !IAST_HPP
