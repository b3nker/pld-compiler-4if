#include "ExprNode.h"
#include "../../CompilerState.h"

ExprNode::ExprNode() {}

ExprNode::~ExprNode() {}

std::string ExprNode::offsetToAsmString(size_t &offset) {
    return "-" + std::to_string(offset) + "(%rbp)";
}

std::string ExprNode::temporarySrcToMemory(std::ostream &out,
                                           std::string &src) {
    auto symbol =
        CompilerState::Get().GetCurrentSymbolTable()->RegisterTemporarySymbol();
    out << "  movl " << src << ", " << offsetToAsmString(symbol->offset)
        << std::endl;
    return offsetToAsmString(symbol->offset);
}

std::string ExprNode::temporarySrcToMemory(std::ostream &out, const char *src) {
    std::string strSrc(src);
    return temporarySrcToMemory(out, strSrc);
}

constexpr bool isRegister(std::string &x) { return (x[0] == '%'); }
constexpr bool isImmediateValue(std::string &x) { return (x[0] == '$'); }

std::string ExprNode::operandEvalAndSaveIfReg(std::ostream &out,
                                              ExprNode *operand) {
    auto op = operand->generateAsm(out);
    if (isRegister(op)) {
        return ExprNode::temporarySrcToMemory(out, op);
    } else {
        return op;
    }
}

std::string ExprNode::operandEvalIntoAnyReg(std::ostream &out,
                                            ExprNode *operand) {
    auto op = operand->generateAsm(out);
    if (isRegister(op)) {
        return op;
    } else {
        std::string reg = "%eax";
        out << "  movl " << op << ", " << reg << std::endl;
        return reg;
    }
}

std::string ExprNode::operandEvalIntoReg(std::ostream &out, ExprNode *operand,
                                         std::string reg) {
    auto op = operand->generateAsm(out);
    if (isRegister(op) && op == reg) {
        return op;
    } else {
        out << "  movl " << op << ", " << reg << std::endl;
        return reg;
    }
}

std::string ExprNode::operandEvalNonImmediate(std::ostream &out,
                                              ExprNode *operand) {
    auto op = operand->generateAsm(out);
    if (isImmediateValue(op)) {
        out << "  movl " << op << ", %eax" << std::endl;
        return ExprNode::temporarySrcToMemory(out, "%eax");
    } else {
        return op;
    }
}

std::string ExprNode::operandEval(std::ostream &out, ExprNode *operand) {
    return operand->generateAsm(out);
}
