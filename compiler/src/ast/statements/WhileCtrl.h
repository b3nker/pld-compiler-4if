#pragma once

#include "../expressions/ExprNode.h"
#include "StatNode.h"

class WhileCtrl : public StatNode {
  public:
    WhileCtrl(ExprNode *condition, StatNode *body);
    virtual ~WhileCtrl();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node* optimize() override;

  private:
    ExprNode *condition;
    StatNode *body;

    size_t uuid;
    static size_t counter;
};
