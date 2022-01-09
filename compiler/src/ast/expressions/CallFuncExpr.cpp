#include "CallFuncExpr.h"
#include "../../CompilerState.h"

CallFuncExpr::CallFuncExpr(std::string funcName,
                           std::vector<ExprNode *> *params)
    : funcName(funcName), params(params){};

CallFuncExpr::~CallFuncExpr() {
    for (auto &&x : *params) {
        delete x;
    }
    delete params;
}

std::string
CallFuncExpr::generateAsmWithTailCallOptimisation(std::ostream &out) {
    return generateAsm(out, true);
}

std::string CallFuncExpr::generateAsm(std::ostream &out) {
    return generateAsm(out, false);
}

std::string CallFuncExpr::generateAsm(std::ostream &out,
                                      bool maybeJumpInsteadOfCall) {
    auto func = CompilerState::Get().functions->GetFunctionForCall(funcName);

    auto nParameters = params->size();

    if (!func) {
        error("Unknown function: '" + funcName + "'");
        return "";
    }

    if (nParameters != func->nParameters) {
        error("Function '" + func->name + "' requires exactly " +
              std::to_string(func->nParameters) + " params but got " +
              std::to_string(nParameters) + ".");
        return "";
    }

    std::vector<std::string> valuesToPush;
    for (auto &&param : *params) {
        // compute parameter
        valuesToPush.push_back(operandEvalAndSaveIfReg(out, param));
    }

    for (size_t j = 0; j < nParameters; j++) {
        const size_t i = nParameters - 1 - j;
        const size_t intParamIndex = i;

        std::string value = valuesToPush[i];

        if (intParamIndex < FUNC_CALL_MAX_INT_PARAMS) {
            out << "  movl  " << value << ", " << FUNC_CALL_REGISTERS[i]
                << "      # Put parameter #" << i << " in register "
                << FUNC_CALL_REGISTERS[i] << std::endl;
        } else {
            out << "  pushq " << value << "      # Push parameter #" << i
                << " on the stack" << std::endl;
        }
    }

    if (maybeJumpInsteadOfCall && canBeTailCallOptimised()) {
        out << "  leave  # restore stack base pointer" << std::endl;
        out << "  jmp   " + funcName + "  # tail call" << std::endl;
    } else {
        out << "  call  " + funcName << std::endl;
    }

    return "%eax";
}

Node *CallFuncExpr::optimize() {
    for (size_t i = 0; i < params->size(); i++) {
        Node *optimizedParam = (*params)[i]->optimize();
        if (optimizedParam) {
            (*params)[i] = dynamic_cast<ExprNode *>(optimizedParam);
        }
    }
    return nullptr;
}
