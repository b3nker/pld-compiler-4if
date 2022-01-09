#include "ReturnStat.h"

#include "../../CompilerState.h"
#include "../../utils/instanceof.h"
#include "../ExpressionNodes.h"

ReturnStat::ReturnStat(ExprNode *expression) : retExpression(expression) {}

ReturnStat::~ReturnStat() { delete retExpression; }

const bool tailCallOptimisationEnabled = true;

std::string ReturnStat::generateAsm(std::ostream &out) {
    if (tailCallOptimisationEnabled) {
        if (instanceof <CallFuncExpr>(retExpression)) {
            auto call = static_cast<CallFuncExpr *>(retExpression);
            if (call->canBeTailCallOptimised()) {
                call->generateAsmWithTailCallOptimisation(out);
                return "";
            }
        }
    }

    std::string output =
        ExprNode::operandEvalIntoReg(out, retExpression, "%eax");
    if (!output.empty() && output != "%eax") {
        out << "  movl " << output << ", %eax       # return value"
            << std::endl;
    }

    out << "  leave" << std::endl;
    out << "  ret" << std::endl;

    return "";
}

Node* ReturnStat::optimize() {
    Node *optimisedExpr = retExpression->optimize();
    if (optimisedExpr) {
        delete retExpression;
        retExpression = dynamic_cast<ExprNode*>(optimisedExpr);
    }
    return nullptr;
}
