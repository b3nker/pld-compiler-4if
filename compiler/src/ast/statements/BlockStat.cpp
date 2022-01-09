#include "BlockStat.h"
#include "ExprStat.h"

BlockStat::BlockStat(std::vector<StatNode *> *statements) : block(statements) {}

BlockStat::~BlockStat() {
    for (StatNode *stat : *block) {
        delete stat;
    }
    delete block;
}

std::string BlockStat::generateAsm(std::ostream &out) {
    for (StatNode *statement : *block) {
        statement->generateAsm(out);
    }
    return "";
}

Node* BlockStat::optimize() {
    for (size_t i = 0; i < block->size(); i++) {
        Node *optimisedStat = (*block)[i]->optimize();
        if (optimisedStat) {
            delete (*block)[i];
            (*block)[i] = dynamic_cast<StatNode*>(optimisedStat);
        }
    }
    return nullptr;
}
