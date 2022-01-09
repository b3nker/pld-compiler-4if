#pragma once

#include "../../types/IfccType.h"
#include "../expressions/AssignExpr.h"
#include "StatNode.h"

class Declaration : public StatNode {
  public:
    Declaration(IfccType *type, std::string identifier, int declLine,
                int declCol);
    Declaration(IfccType *type, std::string identifier, AssignExpr *assignExpr,
                int declLine, int declCol);
    virtual ~Declaration();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node* optimize() override;

  private:
    IfccType *type;
    std::string identifier;
    AssignExpr *initialiser;
    int declarationLine;
    int declarationCol;
};
