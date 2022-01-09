#include "OrExpr.h"

size_t OrExpr::counter = 0;

OrExpr::OrExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {
    uuid = OrExpr::counter++;
}

std::string OrExpr::generateAsm(std::ostream &out) {
    std::string labelFail = ".or" + std::to_string(uuid) + "_fail";
    std::string labelPass = ".or" + std::to_string(uuid) + "_pass";
    std::string labelEnd = ".or" + std::to_string(uuid) + "_end";

    auto op1 = operandEvalNonImmediate(out, operand1);

    out << "  cmpl $0, " << op1 << std::endl;
    out << "  jne " << labelPass << std::endl;

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

intmax_t OrExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 || operand2;
}

bool OrExpr::isNeutralAsOp1(intmax_t value) {
    return value == 0;
}

bool OrExpr::isNeutralAsOp2(intmax_t value){
    return value == 0;
}
