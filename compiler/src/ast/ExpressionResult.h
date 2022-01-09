#pragma once

#include <iostream>
#include <string>

#include "../symbols/SymbolTableEntry.h"
#include "../types/IfccTypes.h"
#include "expressions/ExprNode.h"

class ExpressionResult {
  public:
    virtual size_t getSize() = 0;
    virtual std::string asAsm() = 0;
    virtual std::string asAsm(size_t targetSize) = 0;
    ExpressionResult();
    virtual ~ExpressionResult();

    friend std::ostream &operator<<(std::ostream &out, ExpressionResult &r);

    static char getSuffix(size_t size);
    static void castMove(std::ostream &out, ExpressionResult *orig,
                         ExpressionResult *dest);
    static void castMove(std::ostream &out, ExpressionResult &orig,
                         ExpressionResult &dest);
    static void noCastUpMove(std::ostream &out, ExpressionResult *orig,
                             ExpressionResult *dest);
    static void noCastUpMove(std::ostream &out, ExpressionResult &orig,
                             ExpressionResult &dest);
};

class ExpressionResultAny : public ExpressionResult {
  private:
    std::string asmString;
    size_t size;

  public:
    size_t getSize();
    std::string asAsm();
    std::string asAsm(size_t targetSize);
    ExpressionResultAny(std::string asmString, size_t size);
    ~ExpressionResultAny();
};

class ExpressionResultRegister : public ExpressionResult {
  private:
    std::string registerName;
    size_t size;

  public:
    size_t getSize();
    std::string asAsm();
    std::string asAsm(size_t targetSize);
    ExpressionResultRegister(std::string registerName);
    ~ExpressionResultRegister();
};

class ExpressionResultSymbol : public ExpressionResult {
  private:
    SymbolTableEntry *symbol;

  public:
    size_t getSize();
    std::string asAsm();
    std::string asAsm(size_t targetSize);
    ExpressionResultSymbol(SymbolTableEntry *symbol);
    ~ExpressionResultSymbol();
};

class ExpressionResultOffsetSize : public ExpressionResult {
  private:
    size_t offset;
    size_t size;

  public:
    size_t getSize();
    std::string asAsm();
    std::string asAsm(size_t targetSize);
    ExpressionResultOffsetSize(size_t offset, size_t size);
    ~ExpressionResultOffsetSize();
};

class ExpressionResultTmpSymbol : public ExpressionResultSymbol {};

/*
enum ExpressionResultStorage { Immediate, Register, Ram, Rom };

class ExpressionResult {
  public:
    const bool readable;
    const bool writable;

    const IfccType *type;
    const ExpressionResultStorage storage;
    const std::string toAsm();
    const std::string moveTo();

    ExpressionResult();
    virtual ~ExpressionResult() { delete type; };
};
*/
