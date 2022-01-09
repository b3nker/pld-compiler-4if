#pragma once

#include "../expressions/ExprNode.h"
#include "StatNode.h"

class ForCtrl : public StatNode {
  public:
    ForCtrl(StatNode *init, ExprNode *condition, ExprNode *progress,
            StatNode *body);
    virtual ~ForCtrl();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node* optimize() override;

  private:
    StatNode *init;
    ExprNode *condition;
    ExprNode *progress;
    StatNode *body;

    size_t uuid;
    static size_t counter;
};
