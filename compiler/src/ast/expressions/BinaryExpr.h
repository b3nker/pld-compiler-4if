#pragma once

#include "ExprNode.h"

class BinaryExpr : public ExprNode {
  public:
    BinaryExpr(ExprNode *operand1, ExprNode *operand2);
    virtual ~BinaryExpr();

    virtual Node* optimize() override;

  protected:
    virtual intmax_t operation(intmax_t operand1, intmax_t operand2) = 0;
    virtual bool isNeutralAsOp1(intmax_t value);
    virtual bool isNeutralAsOp2(intmax_t value);

    ExprNode *operand1, *operand2;
};
