#include "AddExpr.h"

AddExpr::AddExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {}

std::string AddExpr::generateAsm(std::ostream &out) {
    auto op1 = operandEvalAndSaveIfReg(out, operand1);
    auto op2 = operandEvalIntoAnyReg(out, operand2);
    out << "  addl " << op1 << ", " << op2 << std::endl;
    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    return op2;
}

intmax_t AddExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 + operand2;
}

bool AddExpr::isNeutralAsOp1(intmax_t value) {
    return value == 0;
}

bool AddExpr::isNeutralAsOp2(intmax_t value){
    return value == 0;
}
