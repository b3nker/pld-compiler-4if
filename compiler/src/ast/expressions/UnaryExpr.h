#pragma once

#include "ExprNode.h"

class UnaryExpr : public ExprNode {
  public:
    UnaryExpr(ExprNode *operand);
    virtual ~UnaryExpr();

    virtual Node* optimize() override;

  protected:
    virtual intmax_t operation(intmax_t operand) = 0;

    ExprNode *operand;
};
