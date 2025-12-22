#include "parser.h"
#include <algorithm>

Parser::Parser(std::string src) : lex(std::move(src))
{
    cur = lex.nextToken();
}

Token Parser::peek()
{
    return cur;
}

Token Parser::consume()
{
    Token t = cur;
    cur = lex.nextToken();
    return t;
}

bool Parser::match(TokenKind k)
{
    if (cur.kind == k)
    {
        consume();
        return true;
    }
    return false;
}

void Parser::expect(TokenKind k, const std::string &msg)
{
    if (cur.kind != k)
    {
        error(msg);
    }
    else
        consume();
}

void Parser::error(const std::string &msg)
{
    std::ostringstream oss;
    oss << "Parse error (line " << cur.line << "): " << msg << " but got '" << cur.text << "'";
    throw std::runtime_error(oss.str());
}

bool Parser::isExpressionStart()
{
    return cur.kind == TokenKind::NUMBER ||
           cur.kind == TokenKind::STRING ||
           cur.kind == TokenKind::IDENT ||
           cur.kind == TokenKind::KW_TRUE ||
           cur.kind == TokenKind::KW_FALSE ||
           cur.kind == TokenKind::LPAREN ||
           cur.kind == TokenKind::MINUS ||
           cur.kind == TokenKind::NOT;
}

std::unique_ptr<Program> Parser::parseProgram()
{
    auto prog = std::make_unique<Program>();
    while (cur.kind != TokenKind::END)
    {
        prog->stmts.push_back(parseStmt());
    }
    return prog;
}

StmtPtr Parser::parseStmt()
{
    if (cur.kind == TokenKind::KW_IF)
        return parseIf();
    if (cur.kind == TokenKind::KW_FOR)
        return parseFor();
    if (cur.kind == TokenKind::KW_OBJ)
        return parseObj();
    if (cur.kind == TokenKind::KW_NUM || cur.kind == TokenKind::KW_STR || cur.kind == TokenKind::KW_BOOL)
        return parseDecl();
    if (cur.kind == TokenKind::KW_BREAK)
    {
        int line = cur.line;
        consume();
        auto body = parseBlock();
        return std::make_unique<BreakStmt>(std::move(body), line);
    }
    if (cur.kind == TokenKind::KW_CONTINUE)
    {
        int line = cur.line;
        consume();
        auto body = parseBlock();
        return std::make_unique<ContinueStmt>(std::move(body), line);
    }

    // Check for assignment: IDENT = expr
    if (cur.kind == TokenKind::IDENT)
    {
        // 需要前瞻来判断是否为赋值语句
        // 创建临时词法分析器来检查下一个token
        Lexer tempLex = lex;                   // 复制当前词法分析器状态
        Token tempToken = tempLex.nextToken(); // 获取下一个token

        if (tempToken.kind == TokenKind::ASSIGN)
        {
            // 这是赋值语句
            int line = cur.line;
            std::string name = cur.text;
            consume(); // consume IDENT
            expect(TokenKind::ASSIGN, "Expected '='");
            auto expr = parseExpr();
            match(TokenKind::SEMI); // Optional semicolon
            return std::make_unique<AssignStmt>(name, std::move(expr), line);
        }
    }

    // Expression statement
    auto expr = parseExpr();
    match(TokenKind::SEMI); // Optional semicolon
    return std::make_unique<ExprStmt>(std::move(expr), expr->line);
}

StmtPtr Parser::parseIf()
{
    int line = cur.line;
    expect(TokenKind::KW_IF, "Expected 'if'");
    expect(TokenKind::LPAREN, "Expected '(' after 'if'");
    auto cond = parseExpr();
    expect(TokenKind::RPAREN, "Expected ')' after if condition");

    auto ifStmt = std::make_unique<IfStmt>(std::move(cond), line);
    ifStmt->thenBody = parseBlock();

    // Handle elif clauses
    while (cur.kind == TokenKind::KW_ELIF)
    {
        consume(); // consume 'elif'
        expect(TokenKind::LPAREN, "Expected '(' after 'elif'");
        auto elifCond = parseExpr();
        expect(TokenKind::RPAREN, "Expected ')' after elif condition");
        auto elifBody = parseBlock();
        ifStmt->elifs.emplace_back(std::move(elifCond), std::move(elifBody));
    }

    // Handle else clause
    if (cur.kind == TokenKind::KW_ELSE)
    {
        consume(); // consume 'else'
        ifStmt->elseBody = parseBlock();
    }

    return ifStmt;
}

StmtPtr Parser::parseFor()
{
    int line = cur.line;
    expect(TokenKind::KW_FOR, "Expected 'for'");
    expect(TokenKind::LPAREN, "Expected '(' after 'for'");

    if (cur.kind != TokenKind::IDENT)
        error("Expected iterator variable name");
    std::string iter = cur.text;
    consume();

    expect(TokenKind::COMMA, "Expected ',' after iterator variable");

    auto forStmt = std::make_unique<ForStmt>(iter, line);

    // Parse arguments (1-3 expressions)
    forStmt->args.push_back(parseExpr());
    if (match(TokenKind::COMMA))
    {
        forStmt->args.push_back(parseExpr());
        if (match(TokenKind::COMMA))
        {
            forStmt->args.push_back(parseExpr());
        }
    }

    expect(TokenKind::RPAREN, "Expected ')' after for arguments");
    forStmt->body = parseBlock();

    return forStmt;
}

StmtPtr Parser::parseObj()
{
    int line = cur.line;
    expect(TokenKind::KW_OBJ, "Expected 'obj'");
    expect(TokenKind::LPAREN, "Expected '(' after 'obj'");

    if (cur.kind != TokenKind::STRING)
        error("Expected class name string");
    std::string className = cur.text;
    consume();

    expect(TokenKind::COMMA, "Expected ',' after class name");
    auto idExpr = parseExpr();
    expect(TokenKind::RPAREN, "Expected ')' after object id");

    auto objStmt = std::make_unique<ObjStmt>(className, std::move(idExpr), line);
    objStmt->body = parseBlock();

    return objStmt;
}

StmtPtr Parser::parseDecl()
{
    int line = cur.line;
    std::string type = cur.text;
    consume(); // consume type keyword

    expect(TokenKind::LPAREN, "Expected '(' after type");

    if (cur.kind != TokenKind::IDENT)
        error("Expected variable name");
    std::string name = cur.text;
    consume();

    expect(TokenKind::RPAREN, "Expected ')' after variable name");

    std::optional<ExprPtr> init;
    std::vector<StmtPtr> initBlock;

    if (match(TokenKind::LBRACE))
    {
        // Check if it's an empty initializer block (default value)
        if (cur.kind != TokenKind::RBRACE)
        {
            // Try to parse as a single expression first
            if (isExpressionStart())
            {
                auto expr = parseExpr();
                if (cur.kind == TokenKind::RBRACE)
                {
                    // Single expression case
                    init = std::move(expr);
                }
                else
                {
                    // Multiple statements case - convert first expression to statement
                    initBlock.push_back(std::make_unique<ExprStmt>(std::move(expr), cur.line));
                    while (cur.kind != TokenKind::RBRACE && cur.kind != TokenKind::END)
                    {
                        initBlock.push_back(parseStmt());
                    }
                }
            }
            else
            {
                // Parse statements in the block
                while (cur.kind != TokenKind::RBRACE && cur.kind != TokenKind::END)
                {
                    initBlock.push_back(parseStmt());
                }
            }
        }
        expect(TokenKind::RBRACE, "Expected '}' after initializer");
    }

    match(TokenKind::SEMI); // Optional semicolon

    // Use block constructor if we have statements, otherwise use expression constructor
    if (!initBlock.empty())
    {
        return std::make_unique<DeclStmt>(type, name, std::move(initBlock), line);
    }
    else
    {
        return std::make_unique<DeclStmt>(type, name, std::move(init), line);
    }
}

std::vector<StmtPtr> Parser::parseBlock()
{
    expect(TokenKind::LBRACE, "Expected '{'");
    std::vector<StmtPtr> stmts;

    while (cur.kind != TokenKind::RBRACE && cur.kind != TokenKind::END)
    {
        stmts.push_back(parseStmt());
    }

    expect(TokenKind::RBRACE, "Expected '}'");
    return stmts;
}