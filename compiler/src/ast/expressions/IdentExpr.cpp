#include "IdentExpr.h"
#include "../../CompilerState.h"

IdentExpr::IdentExpr(std::string variableName) : variableName(variableName) {}

std::string IdentExpr::generateAsm(std::ostream &out) {
    auto symt = CompilerState::Get().GetCurrentSymbolTable();
    auto symbol = symt->GetSymbolRightValue(variableName);
    if (!symbol) {
        error("Undefined symbol: " + variableName);
    }
    return offsetToAsmString(symbol->offset);
}
