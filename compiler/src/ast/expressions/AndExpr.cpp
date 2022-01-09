#include "AndExpr.h"

size_t AndExpr::counter = 0;

AndExpr::AndExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {
    uuid = AndExpr::counter++;
}

std::string AndExpr::generateAsm(std::ostream &out) {
    std::string labelFail = ".and" + std::to_string(uuid) + "_fail";
    std::string labelPass = ".and" + std::to_string(uuid) + "_pass";
    std::string labelEnd = ".and" + std::to_string(uuid) + "_end";

    auto op1 = operandEvalNonImmediate(out, operand1);

    out << "  cmpl $0, " << op1 << std::endl;
    out << "  je " << labelFail << std::endl;

    auto op2 = operandEvalNonImmediate(out, operand2);

    out << "  cmpl $0, " << op2 << std::endl;
    out << "  jne " << labelPass << std::endl;
    out << labelFail << ":" << std::endl;
    out << "  movl $0, %eax" << std::endl;
    out << "  jmp " << labelEnd << std::endl;
    out << labelPass << ":" << std::endl;
    out << "  movl $1, %eax" << std::endl;
    out << labelEnd << ":" << std::endl;

    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op2);
    return "%eax";
}

intmax_t AndExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 && operand2;
}

bool AndExpr::isNeutralAsOp1(intmax_t value) {
    return value;
}

bool AndExpr::isNeutralAsOp2(intmax_t value){
    return value;
}
