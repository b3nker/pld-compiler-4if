#pragma once

#include "BinaryExpr.h"

class SubExpr : public BinaryExpr {
  public:
    SubExpr(ExprNode *operand1, ExprNode *operand2);

    virtual std::string generateAsm(std::ostream &out) override;

  protected:
    virtual intmax_t operation(intmax_t operand1, intmax_t operand2) override;
    virtual bool isNeutralAsOp1(intmax_t value) override;
    virtual bool isNeutralAsOp2(intmax_t value) override;
};
