#include "lexer.h"
#include <cctype>

Token::Token(TokenKind k, std::string t, int l) : kind(k), text(std::move(t)), line(l) {}

Lexer::Lexer(std::string s) : src(std::move(s)) {}

char Lexer::peek() const
{
    if (i < src.size())
        return src[i];
    return '\0';
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
            // 跳过注释
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
    // 结束Token
    if (c == '\0')
        return Token(TokenKind::END, "", line);

    // 单字符Token
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

    // 双字符Token
    if (c == '=')
    {
        get();
        if (peek() == '=')
        {
            get();
            return Token(TokenKind::EQ, "==", line);
        }
        return Token(TokenKind::ASSIGN, "=", line); // TODO 这里有赋值符号的token
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

    // 标识符或保留字
    if (std::isalpha(c) || c == '_')
    {
        std::string s;
        while (std::isalnum(peek()) || peek() == '_')
            s.push_back(get());

        // 检查保留字
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
        if (s == "num")
            return Token(TokenKind::KW_NUM, s, line);
        if (s == "str")
            return Token(TokenKind::KW_STR, s, line);
        if (s == "bool")
            return Token(TokenKind::KW_BOOL, s, line);
        if (s == "break")
            return Token(TokenKind::KW_BREAK, s, line);
        if (s == "continue")
            return Token(TokenKind::KW_CONTINUE, s, line);
        if (s == "true")
            return Token(TokenKind::KW_TRUE, s, line);
        if (s == "false")
            return Token(TokenKind::KW_FALSE, s, line);

        return Token(TokenKind::IDENT, s, line);
    }

    // Numbers
    if (std::isdigit(c))
    {
        std::string s;
        while (std::isdigit(peek()))
            s.push_back(get());

        // 检查是否有小数点
        if (peek() == '.')
        {
            // 先预览小数点后是否有数字
            size_t saved_pos = i;
            get(); // 消费小数点

            if (std::isdigit(peek()))
            {
                // 有效的小数, 添加小数点和小数部分
                s.push_back('.');
                while (std::isdigit(peek()))
                    s.push_back(get());
            }
            else
            {
                // 小数点后没有数字, 回退
                i = saved_pos;
            }
        }

        return Token(TokenKind::NUMBER, s, line);
    }

    // Strings
    if (c == '"')
    {
        get(); // 消费 "
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

    // 未知Token
    get();
    return Token(TokenKind::UNKNOWN, std::string(1, c), line);
}