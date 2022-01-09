#include "MinusExpr.h"

MinusExpr::MinusExpr(ExprNode *operand) : UnaryExpr(operand) {}

std::string MinusExpr::generateAsm(std::ostream &out) {
    auto op = operandEvalIntoAnyReg(out, operand);
    out << "  neg  " << op << std::endl;
    return op;
}

intmax_t MinusExpr::operation(intmax_t operand) { return -operand; }
