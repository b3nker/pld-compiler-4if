#include "BitwiseXorExpr.h"

BitwiseXorExpr::BitwiseXorExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {}

std::string BitwiseXorExpr::generateAsm(std::ostream &out) {
    auto op1 = operandEvalAndSaveIfReg(out, operand1);
    auto op2 = operandEvalIntoAnyReg(out, operand2);
    out << "  xorl " << op1 << ", " << op2 << std::endl;
    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    return op2;
}

intmax_t BitwiseXorExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 ^ operand2;
}

bool BitwiseXorExpr::isNeutralAsOp1(intmax_t value) {
    return false;
}

bool BitwiseXorExpr::isNeutralAsOp2(intmax_t value){
    return false;
}
