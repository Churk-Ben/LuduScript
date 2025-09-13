#include "lexer.h"
#include <cctype>

Token::Token(TokenKind k, std::string t, int l) : kind(k), text(std::move(t)), line(l) {}

Lexer::Lexer(std::string s) : src(std::move(s)) {}

char Lexer::peek() const
{
    return i < src.size() ? src[i] : '\0';
}

char Lexer::get()
{
    if (i < src.size())
        return src[i++];
    return '\0';
}

void Lexer::skipWhitespace()
{
    while (true)
    {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r')
        {
            get();
        }
        else if (c == '\n')
        {
            get();
            line++;
        }
        else if (c == '/' && i + 1 < src.size() && src[i + 1] == '/')
        {
            // Skip line comment
            while (peek() != '\n' && peek() != '\0')
                get();
        }
        else
        {
            break;
        }
    }
}

Token Lexer::nextToken()
{
    skipWhitespace();
    char c = peek();
    if (c == '\0')
        return Token(TokenKind::END, "", line);
    
    // Single character tokens
    if (c == '(')
    {
        get();
        return Token(TokenKind::LPAREN, "(", line);
    }
    if (c == ')')
    {
        get();
        return Token(TokenKind::RPAREN, ")", line);
    }
    if (c == '{')
    {
        get();
        return Token(TokenKind::LBRACE, "{", line);
    }
    if (c == '}')
    {
        get();
        return Token(TokenKind::RBRACE, "}", line);
    }
    if (c == ',')
    {
        get();
        return Token(TokenKind::COMMA, ",", line);
    }
    if (c == ';')
    {
        get();
        return Token(TokenKind::SEMI, ";", line);
    }
    if (c == '.')
    {
        get();
        return Token(TokenKind::DOT, ".", line);
    }
    if (c == '+')
    {
        get();
        return Token(TokenKind::PLUS, "+", line);
    }
    if (c == '-')
    {
        get();
        return Token(TokenKind::MINUS, "-", line);
    }
    if (c == '*')
    {
        get();
        return Token(TokenKind::MUL, "*", line);
    }
    if (c == '/')
    {
        get();
        return Token(TokenKind::DIV, "/", line);
    }
    if (c == '%')
    {
        get();
        return Token(TokenKind::MOD, "%", line);
    }
    
    // Two character tokens
    if (c == '=')
    {
        get();
        if (peek() == '=')
        {
            get();
            return Token(TokenKind::EQ, "==", line);
        }
        return Token(TokenKind::ASSIGN, "=", line);
    }
    if (c == '!')
    {
        get();
        if (peek() == '=')
        {
            get();
            return Token(TokenKind::NEQ, "!=", line);
        }
        return Token(TokenKind::NOT, "!", line);
    }
    if (c == '<')
    {
        get();
        if (peek() == '=')
        {
            get();
            return Token(TokenKind::LE, "<=", line);
        }
        return Token(TokenKind::LT, "<", line);
    }
    if (c == '>')
    {
        get();
        if (peek() == '=')
        {
            get();
            return Token(TokenKind::GE, ">=", line);
        }
        return Token(TokenKind::GT, ">", line);
    }
    if (c == '&' && i + 1 < src.size() && src[i + 1] == '&')
    {
        get();
        get();
        return Token(TokenKind::AND, "&&", line);
    }
    if (c == '|' && i + 1 < src.size() && src[i + 1] == '|')
    {
        get();
        get();
        return Token(TokenKind::OR, "||", line);
    }
    
    // Identifiers and keywords
    if (std::isalpha(c) || c == '_')
    {
        std::string s;
        while (std::isalnum(peek()) || peek() == '_')
            s.push_back(get());
        
        // Check for keywords
        if (s == "if")
            return Token(TokenKind::KW_IF, s, line);
        if (s == "elif")
            return Token(TokenKind::KW_ELIF, s, line);
        if (s == "else")
            return Token(TokenKind::KW_ELSE, s, line);
        if (s == "for")
            return Token(TokenKind::KW_FOR, s, line);
        if (s == "obj")
            return Token(TokenKind::KW_OBJ, s, line);
        if (s == "int")
            return Token(TokenKind::KW_INT, s, line);
        if (s == "str")
            return Token(TokenKind::KW_STR, s, line);
        if (s == "bool")
            return Token(TokenKind::KW_BOOL, s, line);
        if (s == "true" || s == "false")
            return Token(TokenKind::IDENT, s, line); // Handle as ident for simplicity
        
        return Token(TokenKind::IDENT, s, line);
    }
    
    // Numbers
    if (std::isdigit(c))
    {
        std::string s;
        while (std::isdigit(peek()))
            s.push_back(get());
        return Token(TokenKind::NUMBER, s, line);
    }
    
    // Strings
    if (c == '"')
    {
        get(); // consume "
        std::string s;
        while (true)
        {
            char ch = get();
            if (ch == '\0')
                return Token(TokenKind::UNKNOWN, s, line);
            if (ch == '"')
                break;
            if (ch == '\\')
            {
                char nx = get();
                if (nx == 'n')
                    s.push_back('\n');
                else if (nx == 't')
                    s.push_back('\t');
                else if (nx == '"')
                    s.push_back('"');
                else if (nx == '\\')
                    s.push_back('\\');
                else
                    s.push_back(nx);
            }
            else
            {
                s.push_back(ch);
            }
        }
        return Token(TokenKind::STRING, s, line);
    }
    
    // Unknown character
    get();
    return Token(TokenKind::UNKNOWN, std::string(1, c), line);
}