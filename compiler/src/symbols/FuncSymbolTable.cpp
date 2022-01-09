#include "FuncSymbolTable.h"
#include "SymbolTable.h"

/* private functions */

FunctionTableEntry *FunctionTable::GetFunction(std::string name) {
    auto kv = table.find(name);
    bool symbolFound = kv != table.end();
    if (symbolFound) {
        return kv->second;
    } else {
        // unknown symbol
        return nullptr;
    }
}

/* public functions */

FunctionTable::FunctionTable() {}

FunctionTable::~FunctionTable() {}

FunctionTableEntry *FunctionTable::RegisterFunction(std::string name,
                                                    unsigned int nParameters,
                                                    int declLine, int declCol) {
    auto entry = new FunctionTableEntry;
    entry->name = name;
    entry->declarationLine = declLine;
    entry->declarationCol = declCol;
    entry->symbolTableScope = new SymbolTable("function<" + name + ">");
    entry->nParameters = nParameters;
    table.insert(std::make_pair(name, entry));
    return entry;
}

FunctionTableEntry *
FunctionTable::RegisterFunction(FunctionTableEntry *funcTableEntry) {
    auto alreadyExisting = GetFunction(funcTableEntry->name);
    if (alreadyExisting) {
        if (!alreadyExisting->hasBody && funcTableEntry->hasBody) {
            // TODO: check params
            table.erase(funcTableEntry->name);
            // remove declaration, replaced by definition
        }
    }

    table.insert(std::make_pair(funcTableEntry->name, funcTableEntry));
    return funcTableEntry;
}

FunctionTableEntry *FunctionTable::GetFunctionForCall(std::string name) {
    FunctionTableEntry *symbol = GetFunction(name);
    if (symbol) {
        symbol->usedRight += 1;
    }
    return symbol;
}

void FunctionTable::checkUnusedFunctions() {
    int nbError = 0, nbWarning = 0;

    std::cerr << std::endl;
    for (std::pair<std::string, FunctionTableEntry *> entry : table) {
        if (entry.first == "main") {
            continue;
        }

        auto symbol = entry.second;
        if (symbol->usedRight < 1 && entry.first[0] != '#') {
            std::cerr << "[WARNING] Function \"" << entry.first << "\" "
                      << "(declared at " << symbol->declarationLine << ":"
                      << symbol->declarationCol << ") "
                      << "defined but not used." << std::endl;
            nbWarning += 1;
        } /* else if (!symbol->hasBody && entry.first[0] != '#') {
            std::cerr << "[ERROR] Symbol \"" << entry.first << "\" "
                      << "(declared at " << symbol->declarationLine << ":"
                      << symbol->declarationCol << ") "
                      << "used without any body." << std::endl;
            nbError += 1;
        } */
    }

    if (nbError > 0) {
        exit(1); // TODO Improve error managemement
    }
}

std::map<std::string, FunctionTableEntry *> FunctionTable::GetTable() {
    return table;
}
