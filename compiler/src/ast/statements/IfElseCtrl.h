#pragma once

#include "../expressions/ExprNode.h"
#include "StatNode.h"

class IfElseCtrl : public StatNode {
  public:
    IfElseCtrl(ExprNode *condition, StatNode *bodyThen, StatNode *bodyElse);
    virtual ~IfElseCtrl();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node* optimize() override;

  private:
    ExprNode *condition;
    StatNode *bodyThen;
    StatNode *bodyElse;

    size_t uuid;
    static size_t counter;
};
