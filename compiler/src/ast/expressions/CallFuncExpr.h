#pragma once

#include <vector>

#include "../statements/FuncDecl.h"
#include "ExprNode.h"

class CallFuncExpr : public ExprNode {
  public:
    CallFuncExpr(std::string funcName, std::vector<ExprNode *> *params);
    virtual ~CallFuncExpr();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node *optimize() override;

    std::string generateAsmWithTailCallOptimisation(std::ostream &out);

    bool canBeTailCallOptimised() {
        return params->size() <= FUNC_CALL_MAX_INT_PARAMS;
    }

  private:
    std::string funcName;
    std::vector<ExprNode *> *params;

    std::string generateAsm(std::ostream &out, bool tailCallOptimised);
};
