#pragma once

#include "ast.h"
#include "nlohmann/json.hpp"
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <vector>

using json = nlohmann::json;

// Value type for runtime values
struct Value
{
    enum class Type
    {
        INT,
        STR,
        BOOL
    } type;
    
    ll ival;
    std::string sval;
    bool bval;
    
    static Value makeInt(ll i);
    static Value makeStr(std::string s);
    static Value makeBool(bool b);
    
    std::string toStr() const;
    ll toInt() const;
    bool toBool() const;
};

// Runtime environment
struct Env
{
    // Variable stack
    std::vector<std::unordered_map<std::string, Value>> stack;
    // Current object being built (if any)
    std::optional<json> current_object;
    // Set of declared object fields
    std::unordered_set<std::string> declared_fields;
    // Output array
    json output = json::array();
    
    void pushScope();
    void popScope();
    void setVar(const std::string &k, const Value &v);
    std::optional<Value> getVar(const std::string &k);
};

// Interpreter class
class Interpreter
{
private:
    Env env;
    
    // Expression evaluation
    Value evalExpr(Expr *e);
    Value evalLiteral(LiteralExpr *lit);
    Value evalIdent(IdentExpr *id);
    Value evalUnary(UnaryExpr *u);
    Value evalBinary(BinaryExpr *b);
    Value evalCall(CallExpr *c);
    Value evalAccess(AccessExpr *a);
    
    // Statement execution
    void execStmt(Stmt *s);
    void execBlock(const std::vector<StmtPtr> &body);
    
public:
    Interpreter() = default;
    
    void execute(Program *program);
    std::string getOutput(bool pretty = false) const;
};