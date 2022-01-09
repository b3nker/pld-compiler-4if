#pragma once

#include <vector>

#include "../../symbols/SymbolTableEntry.h"
#include "ExprNode.h"

class LvalueExpr : public ExprNode {
  public:
    LvalueExpr(std::string identifier);
    LvalueExpr(std::string identifier,
               std::vector<ExprNode *> *memberAccessIndices);
    virtual ~LvalueExpr();

    virtual std::string generateAsm(std::ostream &out) override;

    std::string generateAsmLValue(std::ostream &out);

  private:
    std::string identifier;
    std::vector<ExprNode *> *memberAccessIndices;

    std::string generateAsmRightOrLeftValue(std::ostream &out, bool isRvalue);
};
