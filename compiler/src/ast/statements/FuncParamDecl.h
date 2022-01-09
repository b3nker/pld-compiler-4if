#pragma once

#include "../../types/IfccTypes.h"
#include "../commons/Node.h"

class FuncParamDecl : public Node {
  public:
    FuncParamDecl(IfccType *type, std::string identifier, int declLine,
                  int declCol);
    virtual ~FuncParamDecl();

    virtual std::string generateAsm(std::ostream &out) override;

  private:
    IfccType *type;
    std::string identifier;
    int declarationLine;
    int declarationCol;

    friend class FuncDecl;
};
