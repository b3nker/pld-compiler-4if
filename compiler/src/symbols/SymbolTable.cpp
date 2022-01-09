#include "SymbolTable.h"

/* private functions */

SymbolTableEntry *SymbolTable::GetSymbol(std::string name) {
    auto kv = table.find(name);
    bool symbolFound = kv != table.end();
    if (symbolFound) {
        return kv->second;
    } else {
        // unknown symbol
        return nullptr;
    }
}

constexpr size_t align(const size_t &size, const size_t &mod) {
    return (size + (mod - 1)) & (-mod);
}

/* public functions */

SymbolTable::SymbolTable() {}

SymbolTable::SymbolTable(std::string scopeName) : scopeName(scopeName) {}

SymbolTable::~SymbolTable() {}

SymbolTableEntry *SymbolTable::RegisterSymbol(std::string name, IfccType *type,
                                              int declLine, int declCol) {
    auto size = type->getSize();
    auto entry = new SymbolTableEntry;
    entry->name = name;
    entry->type = type;
    entry->size = size;
    entry->offset = align(nextAvailableOffset, size) + size;
    entry->declarationLine = declLine;
    entry->declarationCol = declCol;

    nextAvailableOffset = entry->offset;
    auto firstEntry = table.find(name);
    if (firstEntry == table.end()) {
        // not found -> declared for the first time
        table.insert(std::make_pair(name, entry));
    } else {
        std::cerr << "[ERROR] Symbol \"" << name << "\" "
                  << "redeclaration (at " << declLine << ":" << declCol << ")"
                  << std::endl
                  << "already declared (at "
                  << firstEntry->second->declarationLine << ":"
                  << firstEntry->second->declarationCol << ")" << std::endl;
        this->nbErrors += 1;
    }

    return entry;
}

SymbolTableEntry *SymbolTable::GetSymbolLeftValue(std::string name) {
    SymbolTableEntry *symbol = GetSymbol(name);
    if (symbol) {
        symbol->initialised = true;
    }
    return symbol;
}

SymbolTableEntry *SymbolTable::GetSymbolRightValue(std::string name) {
    SymbolTableEntry *symbol = GetSymbol(name);
    if (symbol) {
        symbol->usedRight += 1;
    }
    return symbol;
}

SymbolTableEntry *SymbolTable::RegisterTemporarySymbol() {
    auto t = new IfccInt32_t();
    return RegisterTemporarySymbol(t);
}

void SymbolTable::FreeIfTemporarySymbol(std::string name) {
    auto symbol = GetSymbol(name);
    if (symbol == nullptr && name[0] == '-') {
        // is a memory offset in assembly like -...(%rbp)
        auto offset = std::stoull(name.substr(1, name.length() - 7));
        for (auto x : table) {
            if (x.second->offset == offset) {
                symbol = x.second;
                break;
            }
        }
    }
    if (symbol && symbol->isTmp) {
        symbol->isTmpFree = true;
    }
}

SymbolTableEntry *SymbolTable::RegisterTemporarySymbol(IfccType *type) {
    for (auto x : table) {
        if (x.second->isTmp && x.second->isTmpFree) {
            if (x.second->type->getSize() == type->getSize()) {
                const auto offset = x.second->offset;
                delete x.second->type;
                delete x.second;
                table.erase(x.first);
                auto entry = new SymbolTableEntry;
                entry->name =
                    "#tmp_" + std::to_string(nextAvailableCounterName);
                entry->size = type->getSize();
                entry->offset = offset;
                entry->type = type;
                entry->isTmp = true;
                entry->isTmpFree = false;
                nextAvailableCounterName++;
                table.insert(std::make_pair(entry->name, entry));
                return entry;
            }
        }
    }

    auto entry = new SymbolTableEntry;
    entry->name = "#tmp_" + std::to_string(nextAvailableCounterName);
    entry->type = type;
    entry->size = type->getSize();
    entry->offset = align(nextAvailableOffset, entry->size) + entry->size;
    entry->isTmp = true;
    nextAvailableOffset = entry->offset;
    nextAvailableCounterName++;
    table.insert(std::make_pair(entry->name, entry));
    return entry;
}

void SymbolTable::checkUnusedSymbols(int &globalNbErrors,
                                     int &globalNbWarnings) {
    for (std::pair<std::string, SymbolTableEntry *> entry : table) {
        auto symbol = entry.second;
        if (symbol->usedRight < 1 && entry.first[0] != '#') {
            std::cerr << "[WARNING] Symbol \"" << entry.first << "\" "
                      << "(declared at " << symbol->declarationLine << ":"
                      << symbol->declarationCol << ", in " << scopeName << ") "
                      << "defined but not used." << std::endl;
            this->nbWarnings += 1;
        } else if (!symbol->initialised && entry.first[0] != '#') {
            std::cerr << "[ERROR] Symbol \"" << entry.first << "\" "
                      << "(declared at " << symbol->declarationLine << ":"
                      << symbol->declarationCol << ", in " << scopeName << ") "
                      << "used without any initialisation." << std::endl;
            this->nbErrors += 1;
        }
    }

    globalNbErrors += nbErrors;
    globalNbWarnings += nbWarnings;
}

size_t SymbolTable::GetSize() { return nextAvailableOffset; }
