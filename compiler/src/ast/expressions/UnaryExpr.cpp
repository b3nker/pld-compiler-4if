#include "UnaryExpr.h"
#include "ConstExpr.h"

UnaryExpr::UnaryExpr(ExprNode *operand) : operand(operand) {}

UnaryExpr::~UnaryExpr() { delete operand; }

Node* UnaryExpr::optimize() {
    Node *returnNode = nullptr;

    Node *optimizedOp = operand->optimize();
    ConstExpr *opAsCst = dynamic_cast<ConstExpr*>(optimizedOp);

    if (opAsCst) {
        intmax_t value = opAsCst->getValue();
        returnNode = new ConstExpr(operation(value));
        delete optimizedOp;
    } else if (optimizedOp) {
        delete operand;
        operand = dynamic_cast<ExprNode*>(optimizedOp);
    }

    return returnNode;
}
