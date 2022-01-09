#pragma once

#include "UnaryExpr.h"

class NotExpr : public UnaryExpr {
  public:
    NotExpr(ExprNode *operand);

    virtual std::string generateAsm(std::ostream &out) override;

  protected:
    virtual intmax_t operation(intmax_t operand) override;
};
