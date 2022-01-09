#include <string>

#include "ExpressionResult.h"

char ExpressionResult::getSuffix(size_t size) {
    switch (size) {
    case 1:
        return 'b';
    case 2:
        return 'w';
    case 4:
        return 'l';
    case 8:
        return 'q';
    default:
        std::cerr << "No suffix for size " << size << std::endl;
        exit(1);
    }
}

void ExpressionResult::castMove(std::ostream &out, ExpressionResult &orig,
                                ExpressionResult &dest) {
    return ExpressionResult::castMove(out, &orig, &dest);
}

void ExpressionResult::castMove(std::ostream &out, ExpressionResult *orig,
                                ExpressionResult *dest) {
    size_t origSize = orig->getSize();
    size_t destSize = dest->getSize();

    char origSuffix = ExpressionResult::getSuffix(origSize);
    char destSuffix = ExpressionResult::getSuffix(destSize);

    if (destSize > origSize) {
        // cast up
        out << "  movz" << origSuffix << destSuffix << " "
            << orig->asAsm(origSize) << ", " << dest->asAsm(destSize)
            << " # cast up" << std::endl;
    } else {
        // no cast or cast down
        out << "  mov" << destSuffix << " " << orig->asAsm(destSize) << ", "
            << dest->asAsm(destSize) << " # no cast or cast down" << std::endl;
    }
}

void ExpressionResult::noCastUpMove(std::ostream &out, ExpressionResult &orig,
                                    ExpressionResult &dest) {
    return ExpressionResult::noCastUpMove(out, &orig, &dest);
}

void ExpressionResult::noCastUpMove(std::ostream &out, ExpressionResult *orig,
                                    ExpressionResult *dest) {
    size_t destSize = dest->getSize();
    char destSuffix = ExpressionResult::getSuffix(destSize);
    out << "  mov" << destSuffix << " " << orig->asAsm(destSize) << ", "
        << dest->asAsm(destSize) << " # no cast or cast down" << std::endl;
}

ExpressionResult::ExpressionResult() {}
ExpressionResult::~ExpressionResult() {}

std::ostream &operator<<(std::ostream &out, ExpressionResult &r) {
    out << r.asAsm(4);
    return out;
}

ExpressionResultRegister::ExpressionResultRegister(std::string registerName)
    : registerName(registerName), size(4) {}
ExpressionResultRegister::~ExpressionResultRegister() {}
size_t ExpressionResultRegister::getSize() { return size; }
std::string ExpressionResultRegister::asAsm() { return asAsm(getSize()); }
std::string ExpressionResultRegister::asAsm(size_t targetSize) {
    switch (targetSize) {
    case 1:
        return "%" + registerName + "l";
    case 2:
        return "%" + registerName + "x";
    case 4:
        return "%e" + registerName + "x";
    case 8:
        return "%r" + registerName + "x";
    default:
        std::cerr << "Unable to cast register '" << registerName << "' to size "
                  << targetSize << std::endl;
        exit(1);
    }
}

ExpressionResultAny::ExpressionResultAny(std::string asmString, size_t size)
    : asmString(asmString), size(size) {}
ExpressionResultAny::~ExpressionResultAny() {}
size_t ExpressionResultAny::getSize() { return size; }
std::string ExpressionResultAny::asAsm() { return asAsm(getSize()); }
std::string ExpressionResultAny::asAsm(size_t targetSize) {
    switch (targetSize) {
    case 1:
    case 2:
    case 4:
    case 8:
        return asmString;
    default:
        std::cerr << "Unable to cast ERAny '" << asmString << "' / size "
                  << size << " to size " << targetSize << std::endl;
        exit(1);
    }
}

ExpressionResultSymbol::ExpressionResultSymbol(SymbolTableEntry *symbol)
    : symbol(symbol) {}
ExpressionResultSymbol::~ExpressionResultSymbol() {}
size_t ExpressionResultSymbol::getSize() { return symbol->size; }
std::string ExpressionResultSymbol::asAsm() { return asAsm(getSize()); }
std::string ExpressionResultSymbol::asAsm(size_t targetSize) {
    switch (targetSize) {
    case 1:
    case 2:
    case 4:
    case 8:
        return "-" + std::to_string(symbol->offset) + "(%rbp)";
    default:
        std::cerr << "Unable to cast ERSymbol '" << symbol->name << "' ("
                  << symbol->type->getTypeName() << ") to size " << targetSize
                  << std::endl;
        exit(1);
    }
}

ExpressionResultOffsetSize::ExpressionResultOffsetSize(size_t offset,
                                                       size_t size)
    : offset(offset), size(size) {}
ExpressionResultOffsetSize::~ExpressionResultOffsetSize() {}
size_t ExpressionResultOffsetSize::getSize() { return size; }
std::string ExpressionResultOffsetSize::asAsm() { return asAsm(getSize()); }
std::string ExpressionResultOffsetSize::asAsm(size_t targetSize) {
    switch (targetSize) {
    case 1:
    case 2:
    case 4:
    case 8:
        return "-" + std::to_string(offset) + "(%rbp)";
    default:
        std::cerr << "Unable to cast EROffsetSize " << offset
                  << " / size=" << size << " to size " << targetSize
                  << std::endl;
        exit(1);
    }
}
