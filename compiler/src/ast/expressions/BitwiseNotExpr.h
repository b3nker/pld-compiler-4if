#pragma once

#include "UnaryExpr.h"

class BitwiseNotExpr : public UnaryExpr {
  public:
    BitwiseNotExpr(ExprNode *op);

    virtual std::string generateAsm(std::ostream &out) override;

  protected:
    virtual intmax_t operation(intmax_t operand) override;
};
