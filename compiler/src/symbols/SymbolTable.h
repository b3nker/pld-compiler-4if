#pragma once

#include "SymbolTableEntry.h"

#include <iostream>
#include <map>

class SymbolTable {
  private:
    /**
     * Maps a Symbol identifier to its corresponding SymbolTableEntry.
     */
    std::map<std::string, SymbolTableEntry *> table;
    unsigned int nextAvailableOffset = 0;
    unsigned int nextAvailableCounterName = 0;
    unsigned int nbErrors = 0;
    unsigned int nbWarnings = 0;
    SymbolTableEntry *GetSymbol(std::string name);
    std::string scopeName = "?";

  public:
    SymbolTable();
    SymbolTable(std::string scopeName);
    ~SymbolTable();

    /**
     * Registers a new symbol in the local SymbolTable
     * @param name Symbol name
     * @param type Symbol type
     * @param declLine Line number where the symbol was declared
     * @param declCol Column number where the symbol was declared
     * @return Corresponding SymbolTableEntry
     */
    SymbolTableEntry *RegisterSymbol(std::string name, IfccType *type,
                                     int declLine, int declCol);
    /**
     * Resolves the Symbol's name in the current scope when used as an L-Value.
     * @param name
     * @return SymbolTableEntry corresponding to the ident name.
     */
    SymbolTableEntry *GetSymbolLeftValue(std::string name);
    /**
     * Resolves the Symbol's name in the current scope when used as an R-Value.
     * @param name
     * @return SymbolTableEntry corresponding to the ident name.
     */
    SymbolTableEntry *GetSymbolRightValue(std::string name);

    /**
     * Registers a new temporary symbol in the SymbolTable.
     * @return Corresponding SymbolTableEntry
     */
    SymbolTableEntry *RegisterTemporarySymbol();
    /**
     * Registers a new temporary symbol of the specified type in the SymbolTable.
     * @return Corresponding SymbolTableEntry
     */
    SymbolTableEntry *RegisterTemporarySymbol(IfccType *type);
    /**
     * Frees temporary symbol with specified name.
     * @param name
     */
    void FreeIfTemporarySymbol(std::string name);

    /**
     * Checks unused symbols in the table. Updates the number of errors and warnings.
     * @param globalNbErrors
     * @param globalNbWarnings
     */
    void checkUnusedSymbols(int &globalNbErrors, int &globalNbWarnings);
    /**
     * @return SymbolTable's size
     */
    size_t GetSize();
};
