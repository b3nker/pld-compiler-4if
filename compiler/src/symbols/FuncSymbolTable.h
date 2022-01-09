#pragma once

#include "FuncSymbolTableEntry.h"

#include <iostream>
#include <map>

class FunctionTable {
  private:
    /**
     * Maps a Symbol identifier to its corresponding SymbolTableEntry.
     */
    std::map<std::string, FunctionTableEntry *> table;

  public:
    FunctionTable();
    ~FunctionTable();

    /**
    * Resolves the function's name in the current scope.
    * @param name
    * @return FuncSymbolTableEntry corresponding to the name.
    */
    FunctionTableEntry *GetFunction(std::string name);
    /**
    * Resolves the function's name in the current scope when the function is called.
    * @param name
    * @return FuncSymbolTableEntry corresponding to the name.
    */
    FunctionTableEntry *GetFunctionForCall(std::string name);
    /**
     * Registers a new function in the FuncSymbolTable
     * @param name Function name
     * @param type Function type
     * @param declLine Line number where the function was declared
     * @param declCol Column number where the function was declared
     * @return Corresponding FuncSymbolTableEntry
     */
    FunctionTableEntry *RegisterFunction(std::string name,
                                         unsigned int nParameters, int declLine,
                                         int declCol);
    /**
     * Registers a new function based on a specified SymbolTableEntry.
     * @param funcTableEntry
     * @return
     */
    FunctionTableEntry *RegisterFunction(FunctionTableEntry *funcTableEntry);
    /**
     * @return The FuncSymbolTable
     */
    std::map<std::string, FunctionTableEntry *> GetTable();
    /**
     * Checks unused functions in the table.
     */
    void checkUnusedFunctions();
};
