#include "NotExpr.h"

NotExpr::NotExpr(ExprNode *operand) : UnaryExpr(operand) {}

std::string NotExpr::generateAsm(std::ostream &out) {
    auto op = operandEvalNonImmediate(out, operand);

    out << "  cmpl      $0, " << op << std::endl;
    out << "  sete      %al" << std::endl;
    out << "  movzbl    %al, %eax" << std::endl;

    return "%eax";
}

intmax_t NotExpr::operation(intmax_t operand) { return !operand; }
