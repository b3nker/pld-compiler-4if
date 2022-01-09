#pragma once

#include "../../CompilerState.h"
#include "../../symbols/SymbolTableEntry.h"
#include "../commons/Node.h"

class ExprNode : public Node {
  public:
    ExprNode();
    virtual ~ExprNode();

    static std::string offsetToAsmString(size_t &offset);
    static std::string temporarySrcToMemory(std::ostream &out,
                                            std::string &src);
    static std::string temporarySrcToMemory(std::ostream &out, const char *src);

    static std::string operandEvalAndSaveIfReg(std::ostream &out,
                                               ExprNode *operand);
    static std::string operandEvalIntoAnyReg(std::ostream &out,
                                             ExprNode *operand);
    static std::string operandEvalIntoReg(std::ostream &out, ExprNode *operand,
                                          std::string reg);
    static std::string operandEvalNonImmediate(std::ostream &out,
                                               ExprNode *operand);
    static std::string operandEval(std::ostream &out, ExprNode *operand);

  private:
};
