#include <iostream>
#include <sstream>
#include <string>

#include "FuncDecl.h"
#include "FuncParamDecl.h"

#include "../../CompilerState.h"
#include "../expressions/ExprNode.h"

size_t alignStack(const size_t &size, const size_t &mod) {
    return (size + (mod - 1)) & (-mod);
}

FuncDecl::FuncDecl(IfccType *returnType, std::string identifier,
                   std::vector<FuncParamDecl *> *parameters)
    : FuncDecl(returnType, identifier, parameters, nullptr){};

FuncDecl::FuncDecl(IfccType *returnType, std::string identifier,
                   std::vector<FuncParamDecl *> *parameters, BlockStat *block)
    : returnType(returnType), identifier(identifier), parameters(parameters),
      block(block) {
    functionTableEntry.name = identifier;
    functionTableEntry.returnType = this->returnType;
    functionTableEntry.hasBody = (block != nullptr);
    functionTableEntry.nParameters = parameters->size();
    functionTableEntry.symbolTableScope =
        new SymbolTable("function<" + identifier + ">");
};

FuncDecl::~FuncDecl() {
    if (parameters) {
        for (auto &&x : *parameters) {
            delete x;
        }
        delete parameters;
    }

    if (block) {
        delete block;
    }
}

std::string FuncDecl::generateAsm(std::ostream &out) {
    CompilerState::Get().functions->RegisterFunction(&functionTableEntry);

    // register global for linker
    out << "\n\n";
    out << "  .globl\t" << identifier << "\n";
    out << "  .type\t" << identifier << ", @function\n";

    if (block) {
        // generate asm

        out << identifier << ":\n"; // label

        // prologue
        out << "  endbr64            # detect control flow violations\n" // optional
               "  pushq %rbp         # save address of previous stack frame\n"
               "  movq  %rsp, %rbp   # address of current stack frame\n";

        CompilerState::Get().PushScope(&functionTableEntry);

        std::stringstream tmpAsmOut;

        size_t i = 0;
        for (auto param : *parameters) {
            const auto intParamIndex = i;

            auto symbol = CompilerState::Get().RegisterSymbolLocal(
                param->identifier, param->type, param->declarationLine,
                param->declarationCol);
            symbol->initialised = true;

            if (intParamIndex < FUNC_CALL_MAX_INT_PARAMS) {
                tmpAsmOut << "  # Retrieve param #" << i << " from register "
                          << FUNC_CALL_REGISTERS[intParamIndex] << std::endl;
                tmpAsmOut << "  movl  " << FUNC_CALL_REGISTERS[intParamIndex]
                          << ", " << ExprNode::offsetToAsmString(symbol->offset)
                          << std::endl;
            } else {
                const size_t stackOffset =
                    16 + 8 * (intParamIndex - FUNC_CALL_MAX_INT_PARAMS);
                tmpAsmOut << "  # Retrieve param #" << i << " from stack"
                          << std::endl;
                tmpAsmOut << "  movl  "
                          << std::to_string(stackOffset) + "(%rbp)"
                          << ", %eax" << std::endl;
                tmpAsmOut << "  movl  %eax, "
                          << ExprNode::offsetToAsmString(symbol->offset)
                          << std::endl;
            }

            i += 1;
        }

        block->generateAsm(tmpAsmOut);

        // compute stack size
        size_t stackSize =
            CompilerState::Get().GetCurrentSymbolTable()->GetSize();
        // the value (%rsp + 8) is always a multiple of 16 (x86_64 ABI)
        size_t alignedStackSize = alignStack(stackSize, 16);
        // if caller-saved registers are pushed on the stack, for each one
        // emit `push` instruction and remove 8 to `alignedStackSize`
        if (alignedStackSize > 0) {
            out << "  subq  $" << alignedStackSize
                << ", %rsp    # space for local + tmp variables\n";
        }

        if (tmpAsmOut.rdbuf()->in_avail()) {
            out << tmpAsmOut.rdbuf();
        }

        CompilerState::Get().PopScope();

        out << "  xorl %eax, %eax # default return code = 0" << std::endl;

        const auto RETURN = "." + identifier + ".return"; // return block
        out << RETURN << ": # default return" << std::endl;

        // always return 0 from main, if no return before
        // can also use the `leave` instruction instead of `mov` + `pop`
        // out << "  movq %rbp, %rsp     # drop current stack frame\n"
        //        "  popq %rbp           # restore saved base pointer\n"
        //        "  ret"
        out << "  leave" << std::endl;
        out << "  ret" << std::endl;
    }

    return "";
}

Node *FuncDecl::optimize() {
    if (block) {
        block->optimize();
    }
    return nullptr;
}
