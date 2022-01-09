#include "ExprStat.h"
#include "BlankStat.h"
#include "../expressions/ConstExpr.h"
#include "../expressions/IdentExpr.h"

ExprStat::ExprStat(ExprNode *expression) : expression(expression) {}

ExprStat::~ExprStat() { delete expression; }

std::string ExprStat::generateAsm(std::ostream &out) {
    expression->generateAsm(out);
    return "";
}

Node* ExprStat::optimize() {
    Node *returnNode = nullptr;

    Node *optimizedExpr = expression->optimize();
    if (optimizedExpr) {
        delete expression;
        expression = dynamic_cast<ExprNode*>(optimizedExpr);
    }

    if (dynamic_cast<ConstExpr*>(expression) || dynamic_cast<IdentExpr*>(expression)) {
        returnNode = new BlankStat();
    }

    return returnNode;
}
