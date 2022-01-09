#pragma once

#include "../expressions/ExprNode.h"
#include "StatNode.h"

class ExprStat : public StatNode {
  public:
    ExprStat(ExprNode *expression);
    virtual ~ExprStat();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node* optimize() override;

  private:
    ExprNode *expression;
};
