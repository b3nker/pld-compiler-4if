#include "IfElseCtrl.h"
#include "../expressions/ConstExpr.h"
#include "BlankStat.h"

size_t IfElseCtrl::counter = 0;

IfElseCtrl::IfElseCtrl(ExprNode *condition, StatNode *bodyThen,
                       StatNode *bodyElse)
    : condition(condition), bodyThen(bodyThen), bodyElse(bodyElse) {
    uuid = IfElseCtrl::counter++;
}

IfElseCtrl::~IfElseCtrl() {
    delete condition;
    delete bodyThen;
    if (bodyElse) {
        delete bodyElse;
    }
}

std::string IfElseCtrl::generateAsm(std::ostream &out) {
    const auto name = ".if" + std::to_string(uuid);
    const auto THEN = name + ".then"; // new basic block (true from entry)
    const auto ELSE = name + ".else"; // new basic block (false from entry)
    const auto END = name + ".end";   // new basic block (from .then/.else)

    out << name << ":" << std::endl; // not a new basic block

    const auto val = ExprNode::operandEvalIntoAnyReg(out, condition);
    out << "  cmp $0, " << val << std::endl;

    if (bodyElse) {
        // if there is no `else` branch,
        // directly jump to END if !condition
        out << "  je  " << ELSE << std::endl;
    } else {
        out << "  je  " << END << std::endl;
    }

    out << THEN << ":" << std::endl;
    if (bodyThen) {
        bodyThen->generateAsm(out);
    }

    if (bodyElse) {
        // if there is no `else` branch, no need to jump
        out << "  jmp " << END << std::endl;
        out << ELSE << ":" << std::endl;
        if (bodyElse) {
            bodyElse->generateAsm(out);
        }
    }

    out << END << ":" << std::endl;
    return "";
}

Node* IfElseCtrl::optimize() {
    Node *returnNode = nullptr;

    Node *optimisedCond = condition->optimize();
    if (optimisedCond) {
        delete condition;
        condition = dynamic_cast<ExprNode*>(optimisedCond);
    }
    Node *optimisedThen = bodyThen->optimize();
    if (optimisedThen) {
        delete bodyThen;
        bodyThen = dynamic_cast<StatNode*>(optimisedThen);
    }
    if (bodyElse) {
        Node *optimisedElse = bodyElse->optimize();
        if (optimisedElse) {
            delete bodyElse;
            bodyElse = dynamic_cast<StatNode*>(optimisedElse);
        }
    }

    ConstExpr *condAsCst = dynamic_cast<ConstExpr*>(condition);
    if (condAsCst) {
        if (condAsCst->getValue() == 0) {
            if (bodyElse) {
                returnNode = bodyElse;
                bodyElse = nullptr;
            } else {
                returnNode = new BlankStat();
            }
        } else {
            returnNode = bodyThen;
            bodyThen = nullptr;
        }
    }

    return returnNode;
}
