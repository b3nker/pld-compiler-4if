#include "BitwiseNotExpr.h"

BitwiseNotExpr::BitwiseNotExpr(ExprNode *operand) : UnaryExpr(operand) {}

std::string BitwiseNotExpr::generateAsm(std::ostream &out) {
    auto op = operandEvalIntoAnyReg(out, operand);
    out << "  notl  " << op << std::endl;
    return op;
}

intmax_t BitwiseNotExpr::operation(intmax_t operand) { return ~operand; }
