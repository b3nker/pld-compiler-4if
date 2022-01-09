#include "CompilerState.h"

CompilerState::CompilerState() {
    ast = nullptr;
    symbolTable = new SymbolTable("global scope");
    functions = new FunctionTable();
};

CompilerState::~CompilerState() {
    delete symbolTable;
    delete functions;
};

CompilerState &CompilerState::Get() {
    static CompilerState singleton;
    return singleton;
}

void CompilerState::PushScope(FunctionTableEntry *func) { scopes.push_back(func); }

void CompilerState::PopScope() {
    auto s = scopes.back();
    s->symbolTableScope->checkUnusedSymbols(globalNbErrors,
                                            globalNbWarnings);
    scopes.pop_back();
}

SymbolTable *CompilerState::GetCurrentSymbolTable() {
    if (scopes.empty()) {
        return GetGlobalSymbolTable();
    }
    return scopes.back()->symbolTableScope;
}

SymbolTable *CompilerState::GetGlobalSymbolTable() { return symbolTable; };

std::string CompilerState::GetScopeName() {
    if (scopes.empty()) {
        return "";
    }
    return scopes.back()->name;
};

SymbolTableEntry *CompilerState::ResolveSymbolLeftValue(std::string ident) {
    SymbolTableEntry *e;

    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto func = *it;
        auto symTable = func->symbolTableScope;

        e = symTable->GetSymbolLeftValue(ident);
        if (e != nullptr) {
            return e;
        }
    }
    e = GetGlobalSymbolTable()->GetSymbolLeftValue(ident);
    return e;
}

SymbolTableEntry *CompilerState::RegisterSymbolLocal(std::string ident, IfccType *type,
                                      int declLine, int declCol) {
    return GetCurrentSymbolTable()->RegisterSymbol(ident, type, declLine,
                                                   declCol);
}

SymbolTableEntry *CompilerState::RegisterTmpSymbolLocal() {
    return GetCurrentSymbolTable()->RegisterTemporarySymbol();
}

SymbolTableEntry *CompilerState::RegisterTmpSymbolLocal(IfccType *type) {
    return GetCurrentSymbolTable()->RegisterTemporarySymbol(type);
}

SymbolTableEntry *CompilerState::ResolveSymbolRightValue(std::string ident) {
    SymbolTableEntry *e;

    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto func = *it;
        auto symTable = func->symbolTableScope;

        e = symTable->GetSymbolRightValue(ident);
        if (e != nullptr) {
            return e;
        }
    }
    e = GetGlobalSymbolTable()->GetSymbolRightValue(ident);
    return e;
}

void CompilerState::summary() {
    if (globalNbErrors > 0 || globalNbWarnings > 0) {
        std::cerr << std::endl;
        std::cerr << "Compilation ended with " << globalNbWarnings
                  << " warnings"
                  << " and " << globalNbErrors << " errors." << std::endl;
        if (globalNbErrors > 0) {
            exit(1); // TODO Improve error management
        }
    }
}