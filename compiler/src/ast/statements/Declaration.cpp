#include "Declaration.h"
#include "../../CompilerState.h"

Declaration::Declaration(IfccType *type, std::string identifier, int declLine,
                         int declCol)
    : Declaration(type, identifier, nullptr, declLine, declCol){};

Declaration::Declaration(IfccType *type, std::string identifier,
                         AssignExpr *assignExpr, int declLine, int declCol)
    : type(type), identifier(identifier), initialiser(assignExpr),
      declarationLine(declLine), declarationCol(declCol){};

Declaration::~Declaration() {
    delete type;
    if (initialiser) {
        delete initialiser;
    }
}

std::string Declaration::generateAsm(std::ostream &out) {
    auto symt = CompilerState::Get().GetCurrentSymbolTable();
    auto symbol =
        symt->RegisterSymbol(identifier, type, declarationLine, declarationCol);

    if (initialiser) {
        initialiser->generateAsm(out);
    } else if (dynamic_cast<const IfccArray_t *>(type) != nullptr) {
        auto length = ((IfccArray_t *)type)->itemCount;
        auto scale = ((IfccArray_t *)type)->itemType->getSize();
        out << "  # Array initialisation of " << identifier << ": "
            << type->getTypeName() << std::endl;
        for (size_t i = 0; i < length; i++) {
            size_t offset = symbol->offset - i * scale;
            out << "  movl $0, " << ExprNode::offsetToAsmString(offset)
                << std::endl;
        }
        symbol->initialised = true;
    }

    return "";
}

Node* Declaration::optimize() {
    if (initialiser) {
        initialiser->optimize();
    }
    return nullptr;
}
