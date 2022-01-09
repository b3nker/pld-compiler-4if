#pragma once

#include "ExprNode.h"
#include "LvalueExpr.h"

class AssignExpr : public ExprNode {
  public:
    AssignExpr(LvalueExpr *lval, ExprNode *rval);
    virtual ~AssignExpr();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node *optimize() override;

  private:
    LvalueExpr *lValue;
    ExprNode *rValue;
};
