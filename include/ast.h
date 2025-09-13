#pragma once

#include <memory>
#include <vector>
#include <string>
#include <optional>

using ll = long long;

// Base AST node
struct Node
{
    int line;
    explicit Node(int l = 1) : line(l) {}
    virtual ~Node() = default;
};

// Expression nodes
struct Expr : Node
{
    explicit Expr(int l = 1) : Node(l) {}
};
using ExprPtr = std::unique_ptr<Expr>;

// Statement nodes
struct Stmt : Node
{
    explicit Stmt(int l = 1) : Node(l) {}
};
using StmtPtr = std::unique_ptr<Stmt>;

// Literal expressions
struct LiteralExpr : Expr
{
    enum class Kind
    {
        NUMBER,
        STRING,
        BOOL
    } kind;
    std::string sval;
    ll ival;
    bool bval;
    
    LiteralExpr(ll v, int l);
    LiteralExpr(std::string s, int l);
    LiteralExpr(bool b, int l);
};

// Identifier expressions
struct IdentExpr : Expr
{
    std::string name;
    IdentExpr(std::string n, int l);
};

// Unary expressions
struct UnaryExpr : Expr
{
    std::string op;
    ExprPtr rhs;
    UnaryExpr(std::string o, ExprPtr r, int l);
};

// Binary expressions
struct BinaryExpr : Expr
{
    std::string op;
    ExprPtr lhs, rhs;
    BinaryExpr(ExprPtr l, std::string o, ExprPtr r, int ln);
};

// Function call expressions
struct CallExpr : Expr
{
    ExprPtr callee;
    std::vector<ExprPtr> args;
    CallExpr(ExprPtr c, std::vector<ExprPtr> a, int l);
};

// Member access expressions
struct AccessExpr : Expr
{
    ExprPtr target;
    std::string member;
    AccessExpr(ExprPtr t, std::string m, int l);
};

// Program (root node)
struct Program : Node
{
    std::vector<StmtPtr> stmts;
    Program();
};

// Expression statement
struct ExprStmt : Stmt
{
    ExprPtr expr;
    ExprStmt(ExprPtr e, int l);
};

// Assignment statement
struct AssignStmt : Stmt
{
    std::string name;
    ExprPtr expr;
    AssignStmt(std::string n, ExprPtr e, int l);
};

// Declaration statement
struct DeclStmt : Stmt
{
    std::string type; // "int", "str", "bool"
    std::string name;
    std::optional<ExprPtr> init;
    DeclStmt(std::string t, std::string n, std::optional<ExprPtr> i, int l);
};

// If statement
struct IfStmt : Stmt
{
    ExprPtr cond;
    std::vector<StmtPtr> thenBody;
    std::vector<std::pair<ExprPtr, std::vector<StmtPtr>>> elifs;
    std::vector<StmtPtr> elseBody;
    IfStmt(ExprPtr c, int l);
};

// For statement
struct ForStmt : Stmt
{
    std::string iter;
    std::vector<ExprPtr> args; // 1~3 args: total or start,end or start,end,step
    std::vector<StmtPtr> body;
    ForStmt(std::string it, int l);
};

// Object statement
struct ObjStmt : Stmt
{
    std::string className;
    ExprPtr idExpr;
    std::vector<StmtPtr> body;
    ObjStmt(std::string c, ExprPtr id, int l);
};