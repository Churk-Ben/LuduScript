#pragma once

#include "lexer.h"
#include "ast.h"
#include <memory>
#include <stdexcept>
#include <sstream>

class Parser
{
private:
    Lexer lex;
    Token cur;

    Token peek();
    Token consume();
    bool match(TokenKind k);
    void expect(TokenKind k, const std::string &msg);
    [[noreturn]] void error(const std::string &msg);
    bool isExpressionStart();

    // Expression parsing
    ExprPtr parseExpr();
    ExprPtr parseLogicalOr();
    ExprPtr parseLogicalAnd();
    ExprPtr parseEquality();
    ExprPtr parseComparison();
    ExprPtr parseAddition();
    ExprPtr parseMultiplication();
    ExprPtr parseUnary();
    ExprPtr parsePrimary();
    ExprPtr parseCall(ExprPtr callee);

    // Statement parsing
    StmtPtr parseStmt();
    StmtPtr parseIf();
    StmtPtr parseFor();
    StmtPtr parseObj();
    StmtPtr parseDecl();
    std::vector<StmtPtr> parseBlock();

public:
    explicit Parser(std::string src);
    std::unique_ptr<Program> parseProgram();
};