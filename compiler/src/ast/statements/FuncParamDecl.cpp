#include "FuncParamDecl.h"

FuncParamDecl::FuncParamDecl(IfccType *type, std::string identifier,
                             int declLine, int declCol)
    : type(type), identifier(identifier), declarationLine(declLine),
      declarationCol(declCol){};

FuncParamDecl::~FuncParamDecl() {}

std::string FuncParamDecl::generateAsm(std::ostream &out) { return ""; }
