#include "parser.h"

ExprPtr Parser::parseExpr()
{
    return parseLogicalOr();
}

ExprPtr Parser::parseLogicalOr()
{
    auto left = parseLogicalAnd();
    
    while (cur.kind == TokenKind::OR)
    {
        std::string op = cur.text;
        int line = cur.line;
        consume();
        auto right = parseLogicalAnd();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), line);
    }
    
    return left;
}

ExprPtr Parser::parseLogicalAnd()
{
    auto left = parseEquality();
    
    while (cur.kind == TokenKind::AND)
    {
        std::string op = cur.text;
        int line = cur.line;
        consume();
        auto right = parseEquality();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), line);
    }
    
    return left;
}

ExprPtr Parser::parseEquality()
{
    auto left = parseComparison();
    
    while (cur.kind == TokenKind::EQ || cur.kind == TokenKind::NEQ)
    {
        std::string op = cur.text;
        int line = cur.line;
        consume();
        auto right = parseComparison();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), line);
    }
    
    return left;
}

ExprPtr Parser::parseComparison()
{
    auto left = parseAddition();
    
    while (cur.kind == TokenKind::LT || cur.kind == TokenKind::GT || 
           cur.kind == TokenKind::LE || cur.kind == TokenKind::GE)
    {
        std::string op = cur.text;
        int line = cur.line;
        consume();
        auto right = parseAddition();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), line);
    }
    
    return left;
}

ExprPtr Parser::parseAddition()
{
    auto left = parseMultiplication();
    
    while (cur.kind == TokenKind::PLUS || cur.kind == TokenKind::MINUS)
    {
        std::string op = cur.text;
        int line = cur.line;
        consume();
        auto right = parseMultiplication();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), line);
    }
    
    return left;
}

ExprPtr Parser::parseMultiplication()
{
    auto left = parseUnary();
    
    while (cur.kind == TokenKind::MUL || cur.kind == TokenKind::DIV || cur.kind == TokenKind::MOD)
    {
        std::string op = cur.text;
        int line = cur.line;
        consume();
        auto right = parseUnary();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), line);
    }
    
    return left;
}

ExprPtr Parser::parseUnary()
{
    if (cur.kind == TokenKind::NOT || cur.kind == TokenKind::MINUS)
    {
        std::string op = cur.text;
        int line = cur.line;
        consume();
        auto right = parseUnary();
        return std::make_unique<UnaryExpr>(op, std::move(right), line);
    }
    
    return parsePrimary();
}

ExprPtr Parser::parsePrimary()
{
    int line = cur.line;
    
    // Numbers (integers)
    if (cur.kind == TokenKind::NUMBER)
    {
        ll value = std::stoll(cur.text);
        consume();
        auto expr = std::make_unique<LiteralExpr>(value, line);
        return parseCall(std::move(expr));
    }
    
    // Floating point numbers
    if (cur.kind == TokenKind::FLOAT)
    {
        double value = std::stod(cur.text);
        consume();
        auto expr = std::make_unique<LiteralExpr>(value, line);
        return parseCall(std::move(expr));
    }
    
    // Strings
    if (cur.kind == TokenKind::STRING)
    {
        std::string value = cur.text;
        consume();
        auto expr = std::make_unique<LiteralExpr>(value, line);
        return parseCall(std::move(expr));
    }
    
    // Identifiers (including true/false)
    if (cur.kind == TokenKind::IDENT)
    {
        std::string name = cur.text;
        consume();
        
        ExprPtr expr;
        if (name == "true")
            expr = std::make_unique<LiteralExpr>(true, line);
        else if (name == "false")
            expr = std::make_unique<LiteralExpr>(false, line);
        else
            expr = std::make_unique<IdentExpr>(name, line);
        
        return parseCall(std::move(expr));
    }
    
    // Parenthesized expressions
    if (cur.kind == TokenKind::LPAREN)
    {
        consume(); // consume '('
        auto expr = parseExpr();
        expect(TokenKind::RPAREN, "Expected ')'");
        return parseCall(std::move(expr));
    }
    
    error("Expected expression");
}

ExprPtr Parser::parseCall(ExprPtr callee)
{
    while (true)
    {
        if (cur.kind == TokenKind::LPAREN)
        {
            // Function call
            int line = cur.line;
            consume(); // consume '('
            
            std::vector<ExprPtr> args;
            if (cur.kind != TokenKind::RPAREN)
            {
                args.push_back(parseExpr());
                while (match(TokenKind::COMMA))
                {
                    args.push_back(parseExpr());
                }
            }
            
            expect(TokenKind::RPAREN, "Expected ')' after arguments");
            callee = std::make_unique<CallExpr>(std::move(callee), std::move(args), line);
        }
        else if (cur.kind == TokenKind::DOT)
        {
            // Member access
            int line = cur.line;
            consume(); // consume '.'
            
            if (cur.kind != TokenKind::IDENT)
                error("Expected member name after '.'");
            
            std::string member = cur.text;
            consume();
            
            callee = std::make_unique<AccessExpr>(std::move(callee), member, line);
        }
        else
        {
            break;
        }
    }
    
    return callee;
}