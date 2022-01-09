#pragma once

#include "ExprNode.h"

class ConstExpr : public ExprNode {
  public:
    ConstExpr(intmax_t value);

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node *optimize() override;
    intmax_t getValue();

  private:
    intmax_t value;
};
