#include "ForCtrl.h"
#include "BlankStat.h"
#include "../expressions/ConstExpr.h"
#include "../../CompilerState.h"

size_t ForCtrl::counter = 0;

ForCtrl::ForCtrl(StatNode *init, ExprNode *condition, ExprNode *progress,
                 StatNode *body)
    : init(init), condition(condition), progress(progress), body(body) {
    uuid = ForCtrl::counter++;
}

ForCtrl::~ForCtrl() {
    if (init)
        delete init;
    if (condition)
        delete condition;
    if (progress)
        delete progress;
    if (body)
        delete body;
}

std::string ForCtrl::generateAsm(std::ostream &out) {
    const auto name = ".for" + std::to_string(uuid);
    const auto LOOP = name + ".loop"; // new basic block (loop from .progress)
    const auto PROGRESS = name + ".progress"; // new basic block (+ continue;)
    const auto END = name + ".end";           // new basic block (+ break;)

    out << name << ":" << std::endl; // not a new basic block

    if (init) {
        init->generateAsm(out);
    }

    out << LOOP << ":" << std::endl;
    if (condition) {
        const auto val = ExprNode::operandEvalIntoAnyReg(out, condition);
        out << "  cmp $0, " << val << std::endl;
        out << "  je  " << END << std::endl;
    } // else infinite loop

    if (body) {
        body->generateAsm(out);
    }

    out << PROGRESS << ":" << std::endl; // jump on `continue`
    if (progress) {
        progress->generateAsm(out);
    }

    out << "  jmp " << LOOP << std::endl;

    out << END << ":" << std::endl; // jump on `break`
    return "";
}

Node* ForCtrl::optimize() {
    Node *returnNode = nullptr;

    if (init) {
        Node *optimisedInit = init->optimize();
        if (optimisedInit) {
            delete init;
            init = dynamic_cast<StatNode*>(optimisedInit);
        }
    }
    if (condition) {
        Node *optimizedCond = condition->optimize();
        if (optimizedCond) {
            delete condition;
            condition = dynamic_cast<ExprNode*>(optimizedCond);
        }
    }
    if (progress) {
        Node *optimizedProgress = progress->optimize();
        if (optimizedProgress) {
            delete progress;
            progress = dynamic_cast<ExprNode*>(optimizedProgress);
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
            if (init) {
                returnNode = init;
                init = nullptr;
            } else {
                returnNode = new BlankStat();
            }
        } else {
            warning(std::to_string(uuid) + "th for's condition always true: infinite loop.");
        }
    }

    return returnNode;
}
