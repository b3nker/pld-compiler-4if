#pragma once

#include <vector>

#include "ast/commons/AST.h"
#include "symbols/FuncSymbolTable.h"
#include "symbols/SymbolTable.h"

/**
 * This class stores and tracks useful properties of the compiler's current
 * state. Contains Symbol and Function tables, manages some warning and errors.
 */
class CompilerState {
    /**
     * Default CompilerState constructor.
     * Initializes symbol and function tables.
     */
    CompilerState();
    /**
     * Default CompilerState destructor.
     * Deletes symbol and function tables.
     */
    ~CompilerState();

  public:
    /**
     * Pointer to the Abstract Syntax Tree.
     */
    AST *ast;
    /**
     * Pointer to the Symbol Table.
     */
    SymbolTable *symbolTable;
    /**
     * Pointer to the Functions Table.
     */
    FunctionTable *functions;
    /**
     * Stack of different scope-related FunctionTableEntry objects.
     */
    std::vector<FunctionTableEntry *> scopes;
    /**
     * Number of global errors.
     */
    int globalNbErrors = 0;
    /**
     * Number of global warnings.
     */
    int globalNbWarnings = 0;

    /**
     * Retrieve the CompilerState singleton
     * @return Singleton of CompilerState
     */
    static CompilerState &Get();

    /**
     * Adds a new scope to the stack of scopes.
     * @param func
     */
    void PushScope(FunctionTableEntry *func);

    /**
     * Pops the last scope of the stack and checks unused symbols.
     */
    void PopScope();

    /**
     * @return Pointer to current SymbolTable.
     */
    SymbolTable *GetCurrentSymbolTable();

    /**
     * @return Pointer to global SymbolTable.
     */
    SymbolTable *GetGlobalSymbolTable();

    /**
     * @return The name of the current scope.
     */
    std::string GetScopeName();

    /**
     * Resolves the Symbol's name in the current scope when used as an L-Value.
     * @param ident
     * @return SymbolTableEntry corresponding to the ident name.
     */
    SymbolTableEntry *ResolveSymbolLeftValue(std::string ident);

    /**
     * Registers a new symbol in the local SymbolTable
     * @param ident Symbol identifier
     * @param type Symbol type
     * @param declLine Line number where the symbol was declared
     * @param declCol Column number where the symbol was declared
     * @return Corresponding SymbolTableEntry
     */
    SymbolTableEntry *RegisterSymbolLocal(std::string ident, IfccType *type,
                                          int declLine, int declCol);

    /**
     * Registers a new temporary symbol in the local SymbolTable
     * @return Corresponding SymbolTableEntry
     */
    SymbolTableEntry *RegisterTmpSymbolLocal();

    /**
     * Registers a new temporary symbol in the local SymbolTable with a
     * specified type.
     * @param type
     * @return
     */
    SymbolTableEntry *RegisterTmpSymbolLocal(IfccType *type);

    /**
     * Resolves the Symbol's name in the current scope when used as a R-Value.
     * @param ident
     * @return SymbolTableEntry corresponding to the ident name.
     */
    SymbolTableEntry *ResolveSymbolRightValue(std::string ident);

    /**
     * Prints a summary of the compilation (errors, warnings, etc.)
     */
    void summary();
};
