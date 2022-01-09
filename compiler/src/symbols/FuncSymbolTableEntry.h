#pragma once

#include "../types/IfccTypes.h"
#include "SymbolTable.h"

#include <string>

struct FunctionTableEntry {
    std::string name;
    IfccType *returnType;
    bool hasBody = true;

    int declarationLine = -1;
    int declarationCol = -1;

    unsigned int usedRight = 0;

    unsigned int nParameters = 0;

    SymbolTable *symbolTableScope;
};
