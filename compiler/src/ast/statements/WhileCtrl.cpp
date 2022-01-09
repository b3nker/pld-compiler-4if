#include "WhileCtrl.h"
#include "../../CompilerState.h"
#include "BlankStat.h"
#include "../expressions/ConstExpr.h"

size_t WhileCtrl::counter = 0;

WhileCtrl::WhileCtrl(ExprNode *condition, StatNode *body)
    : condition(condition), body(body) {
    uuid = WhileCtrl::counter++;
}

WhileCtrl::~WhileCtrl() {
    if (condition)
        delete condition;
    if (body)
        delete body;
}

std::string WhileCtrl::generateAsm(std::ostream &out) {
    const auto name = ".while" + std::to_string(uuid);
    const auto LOOP = name + ".loop"; // new basic block (loop from self)
    const auto END = name + ".end";   // new basic block (+ break;)

    out << name << ":" << std::endl; // not a new basic block

    out << LOOP << ":" << std::endl;
    const auto val = ExprNode::operandEvalIntoAnyReg(out, condition);
    out << "  cmp $0, " << val << std::endl;
    out << "  je  " << END << "  # skip body if !condition" << std::endl;

    body->generateAsm(out); // execute body

    out << "  jmp " << LOOP << " # loop" << std::endl;

    out << END << ":" << std::endl;
    return "";
}

Node* WhileCtrl::optimize() {
    Node *returnNode = nullptr;

    if (condition) {
        Node *optimizedCond = condition->optimize();
        if (optimizedCond) {
            delete condition;
            condition = dynamic_cast<ExprNode*>(optimizedCond);
        }
    }
    if (body) {
        Node *optimisedBody = body->optimize();
        if (optimisedBody) {
            delete body;
            body = dynamic_cast<StatNode*>(optimisedBody);
        }
    }

    ConstExpr *condAsCst = dynamic_cast<ConstExpr*>(condition);
    if (condAsCst) {
        if (condAsCst->getValue() == 0) {
            returnNode = new BlankStat();
        } else {
            warning(std::to_string(uuid) + "th while's condition always true: infinite loop.");
        }
    }

    return returnNode;
}
