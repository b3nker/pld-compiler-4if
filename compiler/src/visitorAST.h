#pragma once

#include "antlr4-generated/ifccBaseVisitor.h"
#include "antlr4-runtime.h"

#include <map>

/**
 * This class provides an implementation of ifccBaseVisitor
 */
class VisitorAST : public ifccBaseVisitor {
  public:
    // General

    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

    virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;

    // Statements

    virtual antlrcpp::Any
    visitDeclareStatement(ifccParser::DeclareStatementContext *ctx) override;

    virtual antlrcpp::Any
    visitReturnStatement(ifccParser::ReturnStatementContext *ctx) override;

    virtual antlrcpp::Any
    visitExprStatement(ifccParser::ExprStatementContext *ctx) override;

    virtual antlrcpp::Any
    visitBlankStatement(ifccParser::BlankStatementContext *ctx) override;

    // Expressions

    virtual antlrcpp::Any
    visitParentExpr(ifccParser::ParentExprContext *ctx) override;

    virtual antlrcpp::Any
    visitIdentExpr(ifccParser::IdentExprContext *ctx) override;

    virtual antlrcpp::Any
    visitLiteralExpr(ifccParser::LiteralExprContext *ctx) override;

    virtual antlrcpp::Any
    visitDecimalLiteral(ifccParser::DecimalLiteralContext *ctx) override;

    virtual antlrcpp::Any
    visitCharLiteral(ifccParser::CharLiteralContext *ctx) override;

    virtual antlrcpp::Any
    visitHexLiteral(ifccParser::HexLiteralContext *ctx) override;

    virtual antlrcpp::Any
    visitUnaryExpr(ifccParser::UnaryExprContext *ctx) override;

    virtual antlrcpp::Any
    visitPreIncrExpr(ifccParser::PreIncrExprContext *ctx) override;

    virtual antlrcpp::Any
    visitMultExpr(ifccParser::MultExprContext *ctx) override;

    virtual antlrcpp::Any
    visitAddExpr(ifccParser::AddExprContext *ctx) override;

    virtual antlrcpp::Any
    visitOrderCompExpr(ifccParser::OrderCompExprContext *ctx) override;

    virtual antlrcpp::Any
    visitEqualCompExpr(ifccParser::EqualCompExprContext *ctx) override;

    virtual antlrcpp::Any
    visitBitwiseAndExpr(ifccParser::BitwiseAndExprContext *ctx) override;

    virtual antlrcpp::Any
    visitBitwiseXorExpr(ifccParser::BitwiseXorExprContext *ctx) override;

    virtual antlrcpp::Any
    visitBitwiseOrExpr(ifccParser::BitwiseOrExprContext *ctx) override;

    virtual antlrcpp::Any
    visitAndExpr(ifccParser::AndExprContext *ctx) override;

    virtual antlrcpp::Any visitOrExpr(ifccParser::OrExprContext *ctx) override;

    virtual antlrcpp::Any
    visitAssignExpr(ifccParser::AssignExprContext *ctx) override;

    virtual antlrcpp::Any
    visitAssignExpression(ifccParser::AssignExpressionContext *ctx) override;

    virtual antlrcpp::Any visitFunctionDeclarationOrDefinition(
        ifccParser::FunctionDeclarationOrDefinitionContext *ctx) override;

    virtual antlrcpp::Any
    visitTypedParameter(ifccParser::TypedParameterContext *ctx) override;

    virtual antlrcpp::Any
    visitCallExpr(ifccParser::CallExprContext *ctx) override;

    virtual antlrcpp::Any visitCallFunctionExpression(
        ifccParser::CallFunctionExpressionContext *ctx) override;

    // Control statements

    virtual antlrcpp::Any
    visitWhileStatement(ifccParser::WhileStatementContext *ctx) override;

    virtual antlrcpp::Any
    visitIfStatement(ifccParser::IfStatementContext *ctx) override;

    virtual antlrcpp::Any
    visitForStatement(ifccParser::ForStatementContext *ctx) override;

    virtual antlrcpp::Any
    visitAtomicType(ifccParser::AtomicTypeContext *ctx) override;

    virtual antlrcpp::Any
    visitArrayMemberExpr(ifccParser::ArrayMemberExprContext *ctx) override;

    virtual antlrcpp::Any visitArrayMemberExpression(
        ifccParser::ArrayMemberExpressionContext *ctx) override;

  private:
    std::map<char, char> charToEscapeChar = {
        {'a', '\a'},  {'b', '\b'}, {'f', '\f'}, {'n', '\n'},
        {'r', '\r'},  {'t', '\t'}, {'v', '\v'}, {'\\', '\\'},
        {'\'', '\''}, {'"', '\"'}, {'?', '\?'}};
};
