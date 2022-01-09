#pragma once

#include "BinaryExpr.h"

class OrExpr : public BinaryExpr {
  public:
    OrExpr(ExprNode *op1, ExprNode *op2);

    virtual std::string generateAsm(std::ostream &out) override;

  protected:
    virtual intmax_t operation(intmax_t operand1, intmax_t operand2) override;
    virtual bool isNeutralAsOp1(intmax_t value) override;
    virtual bool isNeutralAsOp2(intmax_t value) override;

  private:
    size_t uuid;
    static size_t counter;
};
