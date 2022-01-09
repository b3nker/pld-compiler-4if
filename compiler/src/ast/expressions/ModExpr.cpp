#include "ModExpr.h"

ModExpr::ModExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {}

std::string ModExpr::generateAsm(std::ostream &out) {
    auto op1 = operandEvalAndSaveIfReg(out, operand1);
    auto op2 = operandEvalIntoReg(out, operand2, "%ebx");

    out << "  movl " << op1 << ", %eax" << std::endl;
    out << "  cdq" << std::endl; // extend the sign bit of eax to edx

    out << "  idivl " << op2 << std::endl;

    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    return "%edx";
}

intmax_t ModExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 % operand2;
}
