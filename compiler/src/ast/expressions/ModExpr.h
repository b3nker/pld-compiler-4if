#pragma once

#include "BinaryExpr.h"

class ModExpr : public BinaryExpr {
  public:
    ModExpr(ExprNode *operand1, ExprNode *operand2);

    virtual std::string generateAsm(std::ostream &out) override;

  protected:
    virtual intmax_t operation(intmax_t operand1, intmax_t operand2) override;
};
