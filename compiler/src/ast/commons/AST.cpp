#include <iostream>

#include "AST.h"

AST::AST(Node *root) : root(root) {}

AST::~AST() { delete root; }

void AST::optimize() {
    root->optimize();
}

void AST::generateAsm(std::ostream &out) { 
    root->generateAsm(out);
}
