#pragma once

#include "../expressions/ExprNode.h"
#include "StatNode.h"

class ReturnStat : public StatNode {
  public:
    ReturnStat(ExprNode *expression);
    virtual ~ReturnStat();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node* optimize() override;

  private:
    ExprNode *retExpression;
};
