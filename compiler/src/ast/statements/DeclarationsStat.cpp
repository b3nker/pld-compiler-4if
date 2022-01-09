#include <vector>

#include "Declaration.h"
#include "DeclarationsStat.h"

DeclarationsStat::DeclarationsStat(std::vector<Declaration *> *declarations)
    : declarations(declarations) {}

DeclarationsStat::~DeclarationsStat() {
    for (auto *decl : *declarations) {
        delete decl;
    }
    delete declarations;
}

std::string DeclarationsStat::generateAsm(std::ostream &out) {
    for (auto *decl : *declarations) {
        decl->generateAsm(out);
    }
    return "";
}

Node* DeclarationsStat::optimize() {
    for (auto *decl : *declarations) {
        decl->optimize();
    }
    return nullptr;
}

