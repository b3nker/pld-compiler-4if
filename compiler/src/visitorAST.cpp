#include <ctype.h>
#include <string>
#include <vector>

#include "CompilerState.h"
#include "ast/ASTNodes.h"
#include "ast/commons/AST.h"
#include "visitorAST.h"

const bool configPrintDebugAst = 0;
const bool configPrintDebugCastAny = 0;

/**
 * Prints the given string on the stderr output if in debug mode
 * @param name String to be printed
 */
void DEBUG_AST(const char *name) {
    if (configPrintDebugAst) {
        std::cerr << name << std::endl;
    }
}

/**
 * It's not yet possible to return AST Node pointers from visitor functions
 * https://github.com/antlr/antlr4/issues/2348
 * @tparam U Mother class (transition)
 * @tparam T Concrete class
 * @param x //TODO
 * @return antlrcpp::Any //TODO
 */
template <typename U = Node, typename T> antlrcpp::Any ifccAny(T *x) {
    if (configPrintDebugCastAny) {
        std::cerr << "ifccAny<" << typeid(T).name() << ", " << typeid(U).name()
                  << ">" << std::endl;
        if (x == nullptr) {
            std::cerr << "  \\-> Warning: ifccAny<" << typeid(T).name() << ", "
                      << typeid(U).name() << ">(nullptr)" << std::endl;
        }
    }
    try {
        return antlrcpp::Any((U *)x);
    } catch (const std::exception &e) {
        if (configPrintDebugCastAny) {
            std::cerr << "EXCEPTION: " << e.what() << std::endl;
        }
        throw e;
    }
}

/**
 * //TODO
 * @tparam T : Class of the object to be returned
 * @tparam U //TODO
 * @param a  The object to cast
 * @return An object of class T
 */
template <typename T, typename U = Node> T *ifccCast(antlrcpp::Any a) {
    if (configPrintDebugCastAny) {
        std::cerr << "ifccCast<" << typeid(T).name() << ", " << typeid(U).name()
                  << ">" << std::endl;
        if (a.isNull()) {
            std::cerr << "  \\-> Warning: ifccCast<" << typeid(T).name() << ", "
                      << typeid(U).name() << ">(Any(nullptr)) " << std::endl;
        }
    }
    try {
        return dynamic_cast<T *>(a.as<U *>());
    } catch (const std::exception &e) {
        if (configPrintDebugCastAny) {
            std::cerr << "EXCEPTION: " << e.what() << std::endl;
        }
        throw e;
    }
}

/**
 * Visit a given tree, if not null, and cast its result into `T*`.
 * If `tree` is nullptr, then return nullptr.
 * Useful to visit optional nodes.
 * @param visitor The visitor
 * @param tree The context to visit if not null
 */
template <typename T>
T *ifccVisitCastNullable(antlr4::tree::ParseTreeVisitor *visitor,
                         antlr4::tree::ParseTree *tree) {
    if (tree == nullptr) {
        return nullptr;
    }
    return ifccCast<T>(visitor->visit(tree));
}

/* Methods of VisitorAST */

// General
/**
 * Visit a ProgContext and start building the AST
 * @param ctx Current ProgContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitProg(ifccParser::ProgContext *ctx) {
    DEBUG_AST("visitProg");
    std::vector<StatNode *> *statements = new std::vector<StatNode *>();
    for (auto child : ctx->topLevelStatement()) {
        auto x = visit(child);
        if (x.isNotNull()) {
            auto childNode = ifccCast<StatNode>(x);
            statements->push_back(childNode);
        }
    }

    auto root = new BlockStat(statements);
    CompilerState::Get().ast = new AST(root);
    return 0;
}

/**
 * Visit a BlockContext and continue building the AST
 * @param ctx Current BlockContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitBlock(ifccParser::BlockContext *ctx) {
    DEBUG_AST("visitBlock");
    std::vector<StatNode *> *statements = new std::vector<StatNode *>();
    for (auto child : ctx->statement()) {
        auto x = visit(child);
        if (x.isNotNull()) {
            auto childNode = ifccCast<StatNode>(x);
            statements->push_back(childNode);
        }
    }
    return ifccAny(new BlockStat(statements));
}

// Statements
/**
 * Visit a DeclareStatementContext and continue buliding the AST
 * @param ctx Current DeclareStatementContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitDeclareStatement(ifccParser::DeclareStatementContext *ctx) {
    DEBUG_AST("visitDeclareStatement");
    IfccType *type = visit(ctx->typePrefix);
    auto declarators = new std::vector<Declaration *>();
    for (auto child : ctx->varDeclaration()) {
        auto ident = child->IDENT()->getText();
        auto expr = child->expression();

        IfccType *childType = type->clone();
        auto indices = child->typeSuffix->arraySuffix()->indices;
        for (auto &&i : indices) {
            if (i) {
                size_t size = std::stoull(i->getText());
                childType = new IfccArray_t(childType, size);
            } else {
                childType = new IfccArray_t(childType);
            }
        }

        size_t line = child->getStart()->getLine();
        size_t col = child->getStart()->getCharPositionInLine();

        Declaration *decl;
        if (expr) {
            auto rValue = ifccCast<ExprNode>(visit(expr));
            auto lValue = new LvalueExpr(ident);
            auto initialiser = new AssignExpr(lValue, rValue);
            decl = new Declaration(childType, ident, initialiser, line, col);
        } else {
            decl = new Declaration(childType, ident, line, col);
        }

        declarators->push_back(decl);
    }
    return ifccAny(new DeclarationsStat(declarators));
}

/**
 * Visit a ReturnStatementContext and continue building the AST
 * @param ctx Current ReturnStatementContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitReturnStatement(ifccParser::ReturnStatementContext *ctx) {
    DEBUG_AST("visitReturnStatement");
    auto returnedExpr = ifccCast<ExprNode>(visit(ctx->expression()));
    return ifccAny(new ReturnStat(returnedExpr));
}

/**
 * Visit a BlankStatementContext //TODO
 * @param ctx current BlankStatementContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitBlankStatement(ifccParser::BlankStatementContext *ctx) {
    DEBUG_AST("visitBlankStatement");
    return ifccAny(new BlankStat());
}

/**
 * Visit an ExprStatementContext and continue building the AST
 * @param ctx Current ExprStatementContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitExprStatement(ifccParser::ExprStatementContext *ctx) {
    DEBUG_AST("visitExprStatement");
    auto node = ifccCast<ExprNode>(visit(ctx->expression()));
    return ifccAny(new ExprStat(node));
}

// Expressions
/**
 * Visit an ParentExprContext and continue building the AST
 * @param ctx Current ParentExprContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitParentExpr(ifccParser::ParentExprContext *ctx) {
    DEBUG_AST("visitParentExpr");
    return visit(ctx->expression());
}

/**
 * Visit an IdentExprContext and continue building the AST
 * @param ctx Current IdentExprContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitIdentExpr(ifccParser::IdentExprContext *ctx) {
    DEBUG_AST("visitIdentExpr");
    return ifccAny(new IdentExpr(ctx->getText()));
}

/**
 * Visit an LiteralExprContext and continue building the AST
 * @param ctx Current LiteralExprContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitLiteralExpr(ifccParser::LiteralExprContext *ctx) {
    DEBUG_AST("visitLiteralExpr");
    return visit(ctx->children[0]);
}

/**
 * Visit an DecimalLiteralContext and continue building the AST
 * @param ctx Current DecimalLiteralContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitDecimalLiteral(ifccParser::DecimalLiteralContext *ctx) {
    DEBUG_AST("visitDecimalLiteral");
    std::string text = ctx->CONST()->getText();
    auto value = std::stoull(text);
    return ifccAny(new ConstExpr(value));
}

/**
 * Visit an CharLiteralContext and continue building the AST
 * @param ctx Current CharLiteralContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitCharLiteral(ifccParser::CharLiteralContext *ctx) {
    DEBUG_AST("visitCharLiteral");
    std::string text = ctx->getText();
    text = text.substr(1, text.length() - 2); // remove quotes

    auto line = ctx->getStart()->getLine();
    auto col = ctx->getStart()->getCharPositionInLine();

    auto firstChar = text[0];
    if (firstChar == '\\') {
        auto secondChar = text[1];
        if (isdigit(secondChar)) {
            // octal number
            std::string stringNumber = text.substr(1); // ignore backslash
            auto number = std::stoull(stringNumber, nullptr, 8);
            if (number > 255) {
                std::cerr << "[WARNING] <" << line << ':' << col
                          << "> Escaped octal out of range: capped at 255"
                          << std::endl;
                CompilerState::Get().globalNbWarnings++;
                return ifccAny(new ConstExpr(255));
            }
            return ifccAny(new ConstExpr(number));
        } else if (secondChar == 'x') {
            // hexadecimal number
            std::string stringNumber = text.substr(2); // ignore backslash and x
            auto number = std::stoull(stringNumber, nullptr, 16);
            if (number > 255) {
                std::cerr << "[WARNING] <" << line << ':' << col
                          << "> Escaped hex out of range: capped at 255"
                          << std::endl;
                CompilerState::Get().globalNbWarnings++;
                return ifccAny(new ConstExpr(255));
            }
            return ifccAny(new ConstExpr(number));
        } else {
            // escaped character
            if (charToEscapeChar.count(secondChar) == 1) {
                // valid escaped char
                char escapedChar = charToEscapeChar[secondChar];
                return ifccAny(new ConstExpr(escapedChar));
            } else {
                // unknown escaped char or sequence
                std::cerr << "[WARNING] <" << line << ':' << col
                          << "> Unknown escape sequence " << text
                          << ": interpreted without escaping" << std::endl;
                CompilerState::Get().globalNbWarnings++;
                return ifccAny(new ConstExpr(secondChar));
            }
        }
    } else {
        // normal character
        return ifccAny(new ConstExpr(firstChar));
    }
}

/**
 * Visit an HexLiteralContext and continue building the AST
 * @param ctx Current HexLiteralContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitHexLiteral(ifccParser::HexLiteralContext *ctx) {
    DEBUG_AST("visitHexLiteral");
    auto value = std::stoull(ctx->getText().substr(2), nullptr, 16);
    return ifccAny(new ConstExpr(value));
}

// antlrcpp::Any
// VisitorAST::visitFloatLiteral(ifccParser::FloatLiteralContext *ctx) {
//     std::cerr << "error: float literals not supported" << std::endl;
//     exit(1);
// }

/**
 * Visit an UnaryExprContext and continue building the AST
 * @param ctx Current UnaryExprContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitUnaryExpr(ifccParser::UnaryExprContext *ctx) {
    DEBUG_AST("visitUnaryExpr");
    ExprNode *retNode;
    auto operand = ifccCast<ExprNode>(visit(ctx->expression()));
    std::string op = ctx->op->getText();

    if (op == "-") {
        retNode = new MinusExpr(operand);
    } else if (op == "!") {
        retNode = new NotExpr(operand);
    } else if (op == "~") {
        retNode = new BitwiseNotExpr(operand);
    } else {
        retNode = nullptr;
    }
    return ifccAny(retNode);
}

/**
 * Visit an PreIncrExprContext and continue building the AST
 * @param ctx Current PreIncrExprContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitPreIncrExpr(ifccParser::PreIncrExprContext *ctx) {
    DEBUG_AST("visitPreIncrExpr");
    ExprNode *retNode;
    auto lval = ifccCast<LvalueExpr>(visit(ctx->id));
    std::string op = ctx->op->getText();

    if (op == "++") {
        retNode = new AssignExpr(lval, new AddExpr(lval, new ConstExpr(1)));
    } else if (op == "--") {
        retNode = new AssignExpr(lval, new SubExpr(lval, new ConstExpr(1)));
    } else {
        retNode = nullptr;
    }
    return ifccAny(retNode);
}

/**
 * Visit an MultExprContext and continue building the AST
 * @param ctx Current MultExprContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitMultExpr(ifccParser::MultExprContext *ctx) {
    DEBUG_AST("visitMultExpr");
    ExprNode *retNode;
    auto operand1 = ifccCast<ExprNode>(visit(ctx->expression(0)));
    auto operand2 = ifccCast<ExprNode>(visit(ctx->expression(1)));
    std::string op = ctx->op->getText();

    if (op == "*") {
        retNode = new MultExpr(operand1, operand2);
    } else if (op == "/") {
        retNode = new DivExpr(operand1, operand2);
    } else if (op == "%") {
        retNode = new ModExpr(operand1, operand2);
    } else {
        retNode = nullptr;
    }
    return ifccAny(retNode);
}

/**
 * Visit an AddExprContext and continue building the AST
 * @param ctx Current AddExprContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitAddExpr(ifccParser::AddExprContext *ctx) {
    DEBUG_AST("visitAddExpr");
    ExprNode *retNode;
    auto operand1 = ifccCast<ExprNode>(visit(ctx->expression(0)));
    auto operand2 = ifccCast<ExprNode>(visit(ctx->expression(1)));
    std::string op = ctx->op->getText();

    if (op == "+") {
        retNode = new AddExpr(operand1, operand2);
    } else if (op == "-") {
        retNode = new SubExpr(operand1, operand2);
    } else {
        retNode = nullptr;
    }
    return ifccAny(retNode);
}

/**
 * Visit an OrderCompExprContext and continue building the AST
 * @param ctx Current OrderCompExprContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitOrderCompExpr(ifccParser::OrderCompExprContext *ctx) {
    DEBUG_AST("visitOrderCompExpr");
    ExprNode *retNode;
    auto operand1 = ifccCast<ExprNode>(visit(ctx->expression(0)));
    auto operand2 = ifccCast<ExprNode>(visit(ctx->expression(1)));
    std::string op = ctx->op->getText();

    if (op == "<") {
        retNode = new LessExpr(operand1, operand2);
    } else if (op == "<=") {
        retNode = new LessOrEqualExpr(operand1, operand2);
    } else if (op == ">") {
        retNode = new GreaterExpr(operand1, operand2);
    } else if (op == ">=") {
        retNode = new GreaterOrEqualExpr(operand1, operand2);
    } else {
        retNode = nullptr;
    }
    return ifccAny(retNode);
}

/**
 * Visit an EqualCompExprContext and continue building the AST
 * @param ctx Current EqualCompExprContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitEqualCompExpr(ifccParser::EqualCompExprContext *ctx) {
    DEBUG_AST("visitEqualCompExpr");
    ExprNode *retNode;
    auto operand1 = ifccCast<ExprNode>(visit(ctx->expression(0)));
    auto operand2 = ifccCast<ExprNode>(visit(ctx->expression(1)));
    std::string op = ctx->op->getText();

    if (op == "==") {
        retNode = new EqualExpr(operand1, operand2);
    } else if (op == "!=") {
        retNode = new NotEqualExpr(operand1, operand2);
    } else {
        retNode = nullptr;
    }
    return ifccAny(retNode);
}

/**
 * Visit an BitwiseAndExprContext and continue building the AST
 * @param ctx Current BitwiseAndExprContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitBitwiseAndExpr(ifccParser::BitwiseAndExprContext *ctx) {
    DEBUG_AST("visitBitwiseAndExpr");
    ExprNode *retNode;
    auto operand1 = ifccCast<ExprNode>(visit(ctx->expression(0)));
    auto operand2 = ifccCast<ExprNode>(visit(ctx->expression(1)));
    retNode = new BitwiseAndExpr(operand1, operand2);
    return ifccAny(retNode);
}

/**
 * Visit an BitwiseXorExprContext and continue building the AST
 * @param ctx Current BitwiseXorExprContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitBitwiseXorExpr(ifccParser::BitwiseXorExprContext *ctx) {
    DEBUG_AST("visitBitwiseXorExpr");
    ExprNode *retNode;
    auto operand1 = ifccCast<ExprNode>(visit(ctx->expression(0)));
    auto operand2 = ifccCast<ExprNode>(visit(ctx->expression(1)));
    retNode = new BitwiseXorExpr(operand1, operand2);
    return ifccAny(retNode);
}

/**
 * Visit an BitwiseOrExprContext and continue building the AST
 * @param ctx Current BitwiseOrExprContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitBitwiseOrExpr(ifccParser::BitwiseOrExprContext *ctx) {
    DEBUG_AST("visitBitwiseOrExpr");
    ExprNode *retNode;
    auto operand1 = ifccCast<ExprNode>(visit(ctx->expression(0)));
    auto operand2 = ifccCast<ExprNode>(visit(ctx->expression(1)));
    retNode = new BitwiseOrExpr(operand1, operand2);
    return ifccAny(retNode);
}

/**
 * Visit an AndExprContext and continue building the AST
 * @param ctx Current AndExprContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitAndExpr(ifccParser::AndExprContext *ctx) {
    DEBUG_AST("visitAndExpr");
    ExprNode *retNode;
    auto operand1 = ifccCast<ExprNode>(visit(ctx->expression(0)));
    auto operand2 = ifccCast<ExprNode>(visit(ctx->expression(1)));
    retNode = new AndExpr(operand1, operand2);
    return ifccAny(retNode);
}

/**
 * Visit an OrExprContext and continue building the AST
 * @param ctx Current OrExprContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitOrExpr(ifccParser::OrExprContext *ctx) {
    DEBUG_AST("visitOrExpr");
    ExprNode *retNode;
    auto operand1 = ifccCast<ExprNode>(visit(ctx->expression(0)));
    auto operand2 = ifccCast<ExprNode>(visit(ctx->expression(1)));
    retNode = new OrExpr(operand1, operand2);
    return ifccAny(retNode);
}

/**
 * Visit an AssignExprContext and continue building the AST
 * @param ctx Current AssignExprContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitAssignExpr(ifccParser::AssignExprContext *ctx) {
    DEBUG_AST("visitAssignExpr");
    return visit(ctx->assignExpression());
}

/**
 * Visit an AssignExpressionContext and continue building the AST
 * @param ctx Current AssignExpressionContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitAssignExpression(ifccParser::AssignExpressionContext *ctx) {
    DEBUG_AST("visitAssignExpression");

    auto leftValue = ifccCast<LvalueExpr>(visit(ctx->arrayMemberExpression()));
    auto rightValue = ifccCast<ExprNode>(visit(ctx->expression()));
    auto node = new AssignExpr(leftValue, rightValue);
    return ifccAny(node);
}

/**
 * Visit an ArrayMemberExprContext and continue building the AST
 * @param ctx Current ArrayMemberExprContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitArrayMemberExpr(ifccParser::ArrayMemberExprContext *ctx) {
    return visit(ctx->arrayMemberExpression());
}

/**
 * Visit an ArrayMemberExpressionContext and continue building the AST
 * @param ctx Current ArrayMemberExpressionContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitArrayMemberExpression(
    ifccParser::ArrayMemberExpressionContext *ctx) {
    std::string identifier = ctx->IDENT()->getText();

    auto indices = ctx->indices;
    auto memberAccessIndices = new std::vector<ExprNode *>();
    for (auto &&indexExpr : indices) {
        auto expressionNode = ifccCast<ExprNode>(visit(indexExpr));
        memberAccessIndices->push_back(expressionNode);
    }
    auto leftValue = new LvalueExpr(identifier, memberAccessIndices);
    return ifccAny(leftValue);
}

/**
 * Visit an FunctionDeclarationOrDefinitionContext and continue building the AST
 * @param ctx Current FunctionDeclarationOrDefinitionContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitFunctionDeclarationOrDefinition(
    ifccParser::FunctionDeclarationOrDefinitionContext *ctx) {
    DEBUG_AST("visitFunctionDeclarationOrDefinition");
    std::string ident = ctx->IDENT()->getText();

    IfccType *returnType;
    if (ctx->returnType->getText() == "void") {
        returnType = new IfccVoid_t();
    } else {
        returnType = visit(ctx->returnType).as<IfccType *>();
    }

    auto params = new std::vector<FuncParamDecl *>();
    for (auto param : ctx->functionDeclParamList()->typedParameter()) {
        params->push_back(ifccCast<FuncParamDecl>(visit(param)));
    }

    auto blockCtx = ctx->block();
    auto block = blockCtx ? ifccCast<BlockStat>(visit(blockCtx)) : nullptr;

    return ifccAny(new FuncDecl(returnType, ident, params, block));
}

/**
 * Visit an TypedParameterContext and continue building the AST
 * @param ctx Current TypedParameterContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitTypedParameter(ifccParser::TypedParameterContext *ctx) {
    DEBUG_AST("visitTypedParameter");
    IfccType *typePrefix =
        ctx->typePrefix ? (IfccType *)(visit(ctx->typePrefix)) : nullptr;

    auto indices = ctx->typeSuffix->arraySuffix()->indices;
    IfccType *type = typePrefix;
    for (auto &&i : indices) {
        if (i) {
            size_t size = std::stoull(i->getText());
            type = new IfccArray_t(type, size);
        } else {
            type = new IfccArray_t(type);
        }
    }

    std::string ident = ctx->IDENT()->getText();
    size_t line = ctx->getStart()->getLine();
    size_t col = ctx->getStart()->getCharPositionInLine();
    return ifccAny(new FuncParamDecl(type, ident, line, col));
}

/**
 * Visit an CallExprContext and continue building the AST
 * @param ctx Current CallExprContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitCallExpr(ifccParser::CallExprContext *ctx) {
    DEBUG_AST("visitCallExpr");
    return visit(ctx->callFunctionExpression());
}

/**
 * Visit an CallFunctionExpressionContext and continue building the AST
 * @param ctx Current CallFunctionExpressionContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitCallFunctionExpression(
    ifccParser::CallFunctionExpressionContext *ctx) {
    DEBUG_AST("visitCallFunctionExpression");
    std::string ident = ctx->IDENT()->getText();

    auto params = new std::vector<ExprNode *>();
    for (auto param : ctx->functionCallParamList()->expression()) {
        params->push_back(ifccCast<ExprNode>(visit(param)));
    }

    return ifccAny(new CallFuncExpr(ident, params));
}

/**
 * Visit an IfStatementContext and continue building the AST
 * @param ctx Current IfStatementContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitIfStatement(ifccParser::IfStatementContext *ctx) {
    DEBUG_AST("visitIfStatement");
    auto condition = ifccCast<ExprNode>(visit(ctx->ifCondition));
    auto bodyThen = ifccCast<StatNode>(visit(ctx->ifBodyThen));
    auto bodyElse = ifccVisitCastNullable<StatNode>(this, ctx->ifBodyElse);
    return ifccAny(new IfElseCtrl(condition, bodyThen, bodyElse));
}

/**
 * Visit an WhileStatementContext and continue building the AST
 * @param ctx Current WhileStatementContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitWhileStatement(ifccParser::WhileStatementContext *ctx) {
    DEBUG_AST("visitWhileStatement");
    auto condition = ifccCast<ExprNode>(visit(ctx->whileCondition));
    auto body = ifccCast<StatNode>(visit(ctx->whileBody));
    return ifccAny(new WhileCtrl(condition, body));
}

/**
 * Visit an ForStatementContext and continue building the AST
 * @param ctx Current ForStatementContext
 * @return //TODO
 */
antlrcpp::Any
VisitorAST::visitForStatement(ifccParser::ForStatementContext *ctx) {
    DEBUG_AST("visitForStatement");
    auto init = ifccVisitCastNullable<StatNode>(this, ctx->forInitialisation);
    auto condition = ifccVisitCastNullable<ExprNode>(this, ctx->forCondition);
    auto progress = ifccVisitCastNullable<ExprNode>(this, ctx->forProgress);
    auto body = ifccVisitCastNullable<StatNode>(this, ctx->forBody);
    return ifccAny(new ForCtrl(init, condition, progress, body));
}

/**
 * Visit an AtomicTypeContext and continue building the AST
 * @param ctx Current AtomicTypeContext
 * @return //TODO
 */
antlrcpp::Any VisitorAST::visitAtomicType(ifccParser::AtomicTypeContext *ctx) {
    std::string t = ctx->getText();
    if (t == "int") {
        return ifccAny<IfccType>(new IfccInt32_t());
    } else if (t == "char") {
        return ifccAny<IfccType>(new IfccInt8_t());
    } else {
        std::cerr << "error: unsupported type '" << t << "'" << std::endl;
        exit(27);
    }
}
