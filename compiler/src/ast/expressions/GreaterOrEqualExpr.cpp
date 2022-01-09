#include "GreaterOrEqualExpr.h"

GreaterOrEqualExpr::GreaterOrEqualExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {}

std::string GreaterOrEqualExpr::generateAsm(std::ostream &out) {
    auto op1 = operandEvalAndSaveIfReg(out, operand1);
    auto op2 = operandEvalIntoReg(out, operand2, "%ebx");

    out << "  movl      " << op1 << ", %eax" << std::endl;
    out << "  cmpl      " << op2 << ", %eax" << std::endl;
    out << "  setge      %al" << std::endl;
    out << "  movzbl    %al, %eax" << std::endl;

    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    return "%eax";
}

intmax_t GreaterOrEqualExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 > operand2;
}
