#include "DivExpr.h"
#include "ConstExpr.h"

DivExpr::DivExpr(ExprNode *operand1, ExprNode *operand2)
    : BinaryExpr(operand1, operand2) {}

std::string DivExpr::generateAsm(std::ostream &out) {
    auto op1 = operandEvalAndSaveIfReg(out, operand1);
    auto op2 = operandEvalIntoReg(out, operand2, "%ebx");

    out << "  movl " << op1 << ", %eax" << std::endl;
    out << "  cdq" << std::endl; // extend the sign bit of eax to edx

    out << "  idivl " << op2 << std::endl;

    CompilerState::Get().GetCurrentSymbolTable()->FreeIfTemporarySymbol(op1);
    return "%eax";
}

intmax_t DivExpr::operation(intmax_t operand1, intmax_t operand2) {
    return operand1 / operand2;
}

Node* DivExpr::optimize() {
    Node *returnNode = nullptr;

    Node *optimizedOp1 = operand1->optimize();
    Node *optimizedOp2 = operand2->optimize();
    ConstExpr *op1AsCst = dynamic_cast<ConstExpr*>(optimizedOp1);
    ConstExpr *op2AsCst = dynamic_cast<ConstExpr*>(optimizedOp2);

    if (op1AsCst && op2AsCst && op2AsCst->getValue() != 0) {
        intmax_t val1 = op1AsCst->getValue();
        intmax_t val2 = op2AsCst->getValue();
        returnNode = new ConstExpr(operation(val1, val2));
        delete optimizedOp1;
        delete optimizedOp2;
    } else if (optimizedOp1 || optimizedOp2) {
        if (optimizedOp1) {
            delete operand1;
            operand1 = dynamic_cast<ExprNode*>(optimizedOp1);
        }
        if (optimizedOp2) {
            delete operand2;
            operand2 = dynamic_cast<ExprNode*>(optimizedOp2);
        }
    }

    if (op2AsCst && isNeutralAsOp2(op2AsCst->getValue())) {
        returnNode = operand1;
        operand1 = nullptr;
    }

    return returnNode;
}

bool DivExpr::isNeutralAsOp1(intmax_t value) {
    return false;
}

bool DivExpr::isNeutralAsOp2(intmax_t value){
    return value == 1;
}
