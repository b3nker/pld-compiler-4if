#pragma once

#include <vector>

#include "../../symbols/FuncSymbolTableEntry.h"
#include "BlockStat.h"
#include "FuncParamDecl.h"
#include "StatNode.h"

const std::string
    FUNC_CALL_REGISTERS[] = // registers for param passing in function calls
    {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
const size_t FUNC_CALL_MAX_INT_PARAMS = 6;

// const std::string
//     FUNC_CALL_XMM_REGISTERS[] = // floating point registers
//     {"%xmm0", "%xmm1", "%xmm2", "%xmm3"};
// const size_t FUNC_CALL_MAX_XMM_PARAMS = 4;

class FuncDecl : public StatNode {
  public:
    FuncDecl(IfccType *returnType, std::string identifier,
             std::vector<FuncParamDecl *> *parameters);
    FuncDecl(IfccType *returnType, std::string identifier,
             std::vector<FuncParamDecl *> *parameters, BlockStat *block);
    virtual ~FuncDecl();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node* optimize() override;

  private:
    IfccType *returnType;
    std::string identifier;
    std::vector<FuncParamDecl *> *parameters;
    BlockStat *block;

    FunctionTableEntry functionTableEntry;
};
