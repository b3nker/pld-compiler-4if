#include "NotEqualExpr.h"

NotEqualExpr::NotEqualExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {}

std::string NotEqualExpr::generateAsm(std::ostream &out) {
    auto op1 = operandEvalAndSaveIfReg(out, operand1);
    auto op2 = operandEvalIntoAnyReg(out, operand2);

    out << "  cmpl      " << op1 << ", " << op2 << std::endl;
    out << "  setne     %al" << std::endl;
    out << "  movzbl    %al, %eax" << std::endl;

    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    return "%eax";
}

intmax_t NotEqualExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 > operand2;
}
