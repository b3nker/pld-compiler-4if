#include "AssignExpr.h"
#include "../../CompilerState.h"
#include "ConstExpr.h"

AssignExpr::AssignExpr(LvalueExpr *lval, ExprNode *rval)
    : lValue(lval), rValue(rval) {}

AssignExpr::~AssignExpr() { delete rValue; }

std::string AssignExpr::generateAsm(std::ostream &out) {
    auto rValueOutput = operandEvalAndSaveIfReg(out, rValue);
    auto lValueOffset = lValue->generateAsmLValue(out);

    out << "  # " << lValueOffset << " = " << rValueOutput << std::endl;

    if (rValueOutput[0] == '$' || rValueOutput[0] == '%') {
        // move from immediate value or register to ram directly
        out << "  movl " << rValueOutput << ", " << lValueOffset << std::endl;
        return rValueOutput;
    }

    // do not modify %eax as it is used for array indexing
    out << "  movl " << rValueOutput << ", %ebx" << std::endl;
    out << "  movl %ebx, " << lValueOffset << std::endl;
    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(
        rValueOutput);
    return lValueOffset;
}

Node *AssignExpr::optimize() {
    Node *optimizedRVal = rValue->optimize();
    if (optimizedRVal) {
        delete rValue;
        rValue = dynamic_cast<ExprNode*>(optimizedRVal);
    }
    return nullptr;
}
