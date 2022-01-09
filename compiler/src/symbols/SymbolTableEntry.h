#pragma once

#include <string>

#include "../types/IfccTypes.h"

struct SymbolTableEntry {
    IfccType *type;
    std::string name;
    size_t size;
    size_t offset;
    int usedRight = 0;
    bool initialised = false;
    int declarationLine = -1;
    int declarationCol = -1;

    bool isTmp = false;
    bool isTmpFree = false;
};
