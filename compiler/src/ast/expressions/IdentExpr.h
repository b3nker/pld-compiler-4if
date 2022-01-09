#pragma once

#include "ExprNode.h"

class IdentExpr : public ExprNode {
  public:
    IdentExpr(std::string variableName);

    virtual std::string generateAsm(std::ostream &out) override;

  private:
    std::string variableName;
};
