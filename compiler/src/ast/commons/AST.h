#pragma once

#include "Node.h"

class AST {
  public:
    AST(Node *root);
    virtual ~AST();

    void optimize();
    void generateAsm(std::ostream &out);

  private:
    Node *root;
};
