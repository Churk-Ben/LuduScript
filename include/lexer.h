#pragma once

#include <string>
#include <vector>

enum class TokenKind
{
    END,
    IDENT,
    // 基本数据类型
    NUMBER,
    STRING,
    BOOL,
    // 关键字
    KW_IF,
    KW_ELIF,
    KW_ELSE,
    KW_FOR,
    KW_BREAK,
    KW_CONTINUE,
    KW_OBJ,
    KW_NUM,
    KW_STR,
    KW_BOOL,
    KW_TRUE,
    KW_FALSE,
    // 符号
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    SEMI,
    DOT,
    // 运算符
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