#include "include/parser.h"
#include "include/lexer.h"
#include <iostream>

int main() {
    std::string input = "num(test_num_default) {}";
    Parser parser(input);
    
    std::cout << "Parsing: " << input << std::endl;
    
    try {
        auto program = parser.parseProgram();
        if (program && !program->stmts.empty()) {
            auto stmt = program->stmts[0].get();
            std::cout << "Successfully parsed statement" << std::endl;
            // 检查语句类型
            if (auto declStmt = dynamic_cast<DeclStmt*>(stmt)) {
                std::cout << "Parsed as DeclStmt - Variable: " << declStmt->name << std::endl;
            } else if (auto exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
                std::cout << "Parsed as ExprStmt" << std::endl;
                if (auto callExpr = dynamic_cast<CallExpr*>(exprStmt->expr.get())) {
                    std::cout << "Contains CallExpr" << std::endl;
                }
            } else {
                std::cout << "Parsed as other statement type" << std::endl;
            }
        } else {
            std::cout << "Failed to parse program or no statements" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Parse error: " << e.what() << std::endl;
    }
    
    return 0;
}