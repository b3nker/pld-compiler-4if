#include "BitwiseAndExpr.h"

BitwiseAndExpr::BitwiseAndExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {}

std::string BitwiseAndExpr::generateAsm(std::ostream &out) {
    auto op1 = operandEvalAndSaveIfReg(out, operand1);
    auto op2 = operandEvalIntoAnyReg(out, operand2);
    out << "  andl " << op1 << ", " << op2 << std::endl;
    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    return op2;
}

intmax_t BitwiseAndExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 & operand2;
}

bool BitwiseAndExpr::isNeutralAsOp1(intmax_t value) {
    return ~value == 0;
}

bool BitwiseAndExpr::isNeutralAsOp2(intmax_t value){
    return ~value == 0;
}
