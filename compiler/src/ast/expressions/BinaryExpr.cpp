#include "BinaryExpr.h"
#include "ConstExpr.h"

BinaryExpr::BinaryExpr(ExprNode *operand1, ExprNode *operand2)
    : operand1(operand1), operand2(operand2) {}

BinaryExpr::~BinaryExpr() {
    delete operand1;
    delete operand2;
}

Node *BinaryExpr::optimize() {
    Node *returnNode = nullptr;

    Node *optimizedOp1 = operand1->optimize();
    Node *optimizedOp2 = operand2->optimize();
    ConstExpr *op1AsCst = dynamic_cast<ConstExpr *>(optimizedOp1);
    ConstExpr *op2AsCst = dynamic_cast<ConstExpr *>(optimizedOp2);

    if (op1AsCst && op2AsCst) {
        intmax_t val1 = op1AsCst->getValue();
        intmax_t val2 = op2AsCst->getValue();
        returnNode = new ConstExpr(operation(val1, val2));
        delete optimizedOp1;
        delete optimizedOp2;
    } else if (optimizedOp1 || optimizedOp2) {
        if (optimizedOp1) {
            delete operand1;
            operand1 = dynamic_cast<ExprNode *>(optimizedOp1);
        }
        if (optimizedOp2) {
            delete operand2;
            operand2 = dynamic_cast<ExprNode *>(optimizedOp2);
        }
    }

    if (op1AsCst && isNeutralAsOp1(op1AsCst->getValue())) {
        returnNode = operand2;
        operand2 = nullptr;
    } else if (op2AsCst && isNeutralAsOp2(op2AsCst->getValue())) {
        returnNode = operand1;
        operand1 = nullptr;
    }

    return returnNode;
}

bool BinaryExpr::isNeutralAsOp1(intmax_t value) { return false; }

bool BinaryExpr::isNeutralAsOp2(intmax_t value) { return false; }
