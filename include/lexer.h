#pragma once

#include <string>
#include <vector>

enum class TokenKind
{
    END,
    IDENT,
    NUMBER,
    STRING,
    KW_IF,
    KW_ELIF,
    KW_ELSE,
    KW_FOR,
    KW_OBJ,
    KW_INT,
    KW_STR,
    KW_BOOL,
    // symbols
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    SEMI,
    DOT,
    // operators
    PLUS,
    MINUS,
    MUL,
    DIV,
    MOD,
    EQ,
    ASSIGN,
    NEQ,
    LT,
    GT,
    LE,
    GE,
    AND,
    OR,
    NOT,
    UNKNOWN
};

struct Token
{
    TokenKind kind;
    std::string text;
    int line;
    
    Token(TokenKind k = TokenKind::UNKNOWN, std::string t = "", int l = 1);
};

class Lexer
{
private:
    std::string src;
    size_t i = 0;
    int line = 1;
    
    char peek() const;
    char get();
    void skipWhitespace();
    
public:
    explicit Lexer(std::string s);
    Token nextToken();
};