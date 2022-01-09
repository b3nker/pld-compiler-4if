#include "BitwiseOrExpr.h"

BitwiseOrExpr::BitwiseOrExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {}

std::string BitwiseOrExpr::generateAsm(std::ostream &out) {
    auto op1 = operandEvalAndSaveIfReg(out, operand1);
    auto op2 = operandEvalIntoAnyReg(out, operand2);
    out << "  orl   " << op1 << ", " << op2 << std::endl;
    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    return op2;
}

intmax_t BitwiseOrExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 | operand2;
}

bool BitwiseOrExpr::isNeutralAsOp1(intmax_t value) {
    return value == 0;
}

bool BitwiseOrExpr::isNeutralAsOp2(intmax_t value){
    return value == 0;
}
