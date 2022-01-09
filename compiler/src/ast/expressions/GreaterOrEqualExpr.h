#pragma once

#include "BinaryExpr.h"

class GreaterOrEqualExpr : public BinaryExpr {
  public:
    GreaterOrEqualExpr(ExprNode *op1, ExprNode *op2);

    virtual std::string generateAsm(std::ostream &out) override;

  protected:
    virtual intmax_t operation(intmax_t operand1, intmax_t operand2) override;
};
