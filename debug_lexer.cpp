#include "include/lexer.h"
#include <iostream>

int main() {
    std::string code = "num(test_num_default)";
    Lexer lex(code);
    
    Token token;
    while ((token = lex.nextToken()).kind != TokenKind::END) {
        std::cout << "Token: " << static_cast<int>(token.kind) << " Text: '" << token.text << "'" << std::endl;
    }
    
    return 0;
}