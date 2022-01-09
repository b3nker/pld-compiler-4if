#include "LvalueExpr.h"
#include "../../CompilerState.h"
#include "../../utils/instanceof.h"
#include "ConstExpr.h"

LvalueExpr::LvalueExpr(std::string identifier)
    : LvalueExpr(identifier, nullptr) {}

LvalueExpr::LvalueExpr(std::string identifier,
                       std::vector<ExprNode *> *memberAccessIndices)
    : identifier(identifier), memberAccessIndices(memberAccessIndices) {}

LvalueExpr::~LvalueExpr() {
    if (memberAccessIndices) {
        for (auto &&expr : *memberAccessIndices) {
            delete expr;
        }
        delete memberAccessIndices;
    }
}

std::string LvalueExpr::generateAsm(std::ostream &out) {
    return generateAsmRightOrLeftValue(out, true);
}

std::string LvalueExpr::generateAsmLValue(std::ostream &out) {
    return generateAsmRightOrLeftValue(out, false);
}

std::string LvalueExpr::generateAsmRightOrLeftValue(std::ostream &out,
                                                    bool isRvalue) {
    SymbolTableEntry *symbol;
    if (isRvalue) {
        symbol = CompilerState::Get().ResolveSymbolRightValue(identifier);
    } else {
        symbol = CompilerState::Get().ResolveSymbolLeftValue(identifier);
    }

    if (!symbol) {
        error("Unknown identifier: '" + identifier + "'");
        exit(25);
    }

    if (memberAccessIndices == nullptr || memberAccessIndices->size() == 0) {
        if (instanceof <IfccArray_t>(symbol->type)) {
            error("Cannot access array variable '" + symbol->name +
                  "'. Indirect memory access not supported.");
        }
        return offsetToAsmString(symbol->offset);
    } else if (memberAccessIndices->size() == 1) {
        if (instanceof <IfccArray_t>(symbol->type)) {
            auto arrayT = (IfccArray_t *)symbol->type;
            // auto arrayLength = arrayT->itemCount;
            auto itemType = arrayT->itemType;
            auto itemSize = itemType->getSize();

            auto indexExpr = memberAccessIndices->at(0);
            if (instanceof <ConstExpr>(indexExpr)) {
                auto constExpr = dynamic_cast<ConstExpr *>(indexExpr);
                intmax_t compileTimeIndex = constExpr->getValue();
                size_t offset = symbol->offset - compileTimeIndex * itemSize;
                return offsetToAsmString(offset);
            }

            std::string index = "%rax";
            auto res = operandEvalIntoReg(out, indexExpr, "%eax");
            out << "  cltq # sign-extend %eax to %rax" << std::endl;
            std::string base = "%rbp";
            std::string displacement = std::to_string(symbol->offset);
            std::string scale = std::to_string(itemSize);
            std::string access = "-" + displacement + "(" + base + ", " +
                                 index + ", " + scale + ")";

            if (isRvalue) {
                // always save if rvalue, because %eax (index) might be modified
                out << "  movl " << access << ", %eax" << std::endl;
                return "%eax";
            }
            return access;
        } else {
            error("Array access to non-array variable.");
            exit(23);
        }
    } else {
        // Multidimensional array
        error("Multidimensional arrays aren't supported.");
        exit(24);
        // out << "  movq %rbp, %rax" << std::endl;
        // out << "  subq $" << symbol->offset << ", %rax" << std::endl;
        // auto accSymbol = CompilerState::Get()
        //                      .GetCurrentSymbolTable()
        //                      ->RegisterTemporarySymbol();
        // auto accumulator = offsetToAsmString(accSymbol->offset);
        // out << "  movq %rax, " << accumulator << std::endl;

        // auto t = symbol->type;
        // for (auto &&expr : *memberAccessIndices) {
        //     // access to member item
        //     // check if current type `t` is IfccArray_t
        //     if (instanceof <IfccArray_t>(t)) {
        //         // auto arrayLength = ((IfccArray_t *)t)->itemCount;
        //         auto itemType = ((IfccArray_t *)t)->itemType;
        //         auto itemSize = itemType->getSize();

        //         auto res = expr->generateAsm(out); // index
        //         out << "  movq " << res << ", %rax" << std::endl;

        //         out << "  imulq $" << itemSize << ", %rax" << std::endl;
        //         out << "  subq %rax, " << accumulator << std::endl;

        //         t = itemType;

        //         // if (arrayLength >= index) {
        //         //     std::cerr << "out of bounds access to value of type "
        //         //               << t->getTypeName() << "(" << index
        //         //               << ">=" << arrayLength << ")" << std::endl;
        //         //     exit(26);
        //         // }

        //     } else {
        //         std::cerr << "invalid type" << std::endl;
        //         exit(28);
        //     }
        // }

        // out << "  leaq " << accumulator << ", %rax" << std::endl;
        // return temporarySrcToMemory(out, "%eax");
    }

    return "";
}
