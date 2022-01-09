#include "SubExpr.h"

SubExpr::SubExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {}

std::string SubExpr::generateAsm(std::ostream &out) {
    auto op1 = operandEvalAndSaveIfReg(out, operand1);
    auto op2 = operandEvalIntoReg(out, operand2, "%ebx");

    out << "  movl " << op1 << ", %eax" << std::endl;
    out << "  subl " << op2 << ", %eax" << std::endl;

    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    return "%eax";
}

intmax_t SubExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 - operand2;
}

bool SubExpr::isNeutralAsOp1(intmax_t value) {
    return false;
}

bool SubExpr::isNeutralAsOp2(intmax_t value){
    return value == 0;
}
