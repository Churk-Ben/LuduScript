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

// Expression nodes 表达式节点
struct Expr : Node
{
    explicit Expr(int l = 1) : Node(l) {}
};
using ExprPtr = std::unique_ptr<Expr>;

// Statement nodes 语句节点
struct Stmt : Node
{
    explicit Stmt(int l = 1) : Node(l) {}
};
using StmtPtr = std::unique_ptr<Stmt>;

// Literal expressions 字面量表达式
struct LiteralExpr : Expr
{
    enum class Kind
    {
        INTEGER, // 整数类型，使用ival
        FLOAT,   // 浮点数类型，使用dval
        STRING,
        BOOL
    } kind;

    union
    {
        ll ival;     // 整数值
        double dval; // 浮点数值
    };
    std::string sval;
    bool bval;

    LiteralExpr(ll v, int l);     // 整数构造函数
    LiteralExpr(double d, int l); // 浮点数构造函数
    LiteralExpr(std::string s, int l);
    LiteralExpr(bool b, int l);
};

// Identifier expressions 标识符表达式
struct IdentExpr : Expr
{
    std::string name;
    IdentExpr(std::string n, int l);
};

// Unary expressions 一元表达式(操作符 + 右操作数)
struct UnaryExpr : Expr
{
    std::string op;
    ExprPtr rhs;
    UnaryExpr(std::string o, ExprPtr r, int l);
};

// Binary expressions 二元表达式(左操作数 + 操作符 + 右操作数)
struct BinaryExpr : Expr
{
    std::string op;
    ExprPtr lhs, rhs;
    BinaryExpr(ExprPtr l, std::string o, ExprPtr r, int ln);
};

// Function call expressions 函数调用表达式(函数名 + 实参列表) TODO 这个似乎解释器还不支持
struct CallExpr : Expr
{
    ExprPtr callee;
    std::vector<ExprPtr> args;
    CallExpr(ExprPtr c, std::vector<ExprPtr> a, int l);
};

// Member access expressions 成员访问表达式(目标对象 + 成员名) TODO 这个似乎解释器还不支持
struct AccessExpr : Expr
{
    ExprPtr target;
    std::string member;
    AccessExpr(ExprPtr t, std::string m, int l);
};

// Program (root node) 程序(根节点)
struct Program : Node
{
    std::vector<StmtPtr> stmts;
    Program();
};

// Expression statement 表达式语句
struct ExprStmt : Stmt
{
    ExprPtr expr;
    ExprStmt(ExprPtr e, int l);
};

// Assignment statement 赋值语句(变量名 + 表达式)
struct AssignStmt : Stmt
{
    std::string name;
    ExprPtr expr;
    AssignStmt(std::string n, ExprPtr e, int l);
};

// Declaration statement 声明语句(类型 + 变量名 + 初始化表达式)
// 声明语句(类型 + 变量名 + 初始化表达式)
// 声明语句(类型 + 变量名 + 初始化语句块)
struct DeclStmt : Stmt
{
    std::string type; // "num", "str", "bool"
    std::string name;
    std::optional<ExprPtr> init;
    std::vector<StmtPtr> initBlock; // For statement block initialization
    DeclStmt(std::string t, std::string n, std::optional<ExprPtr> i, int l);
    DeclStmt(std::string t, std::string n, std::vector<StmtPtr> block, int l);
};

// If statement 条件语句(条件 + 语句块 + 可选的elif语句块 + 可选的else语句块)
struct IfStmt : Stmt
{
    ExprPtr cond;
    std::vector<StmtPtr> thenBody;
    std::vector<std::pair<ExprPtr, std::vector<StmtPtr>>> elifs;
    std::vector<StmtPtr> elseBody;
    IfStmt(ExprPtr c, int l);
};

// For statement 循环语句(迭代变量 + 迭代范围 + 语句块)
struct ForStmt : Stmt
{
    std::string iter;
    std::vector<ExprPtr> args; // 1~3 args: total or start,end or start,end,step
    std::vector<StmtPtr> body;
    ForStmt(std::string it, int l);
};

// Object statement 对象语句(类名 + 可选的对象ID + 语句块)
struct ObjStmt : Stmt
{
    std::string className;
    ExprPtr idExpr;
    std::vector<StmtPtr> body;
    ObjStmt(std::string c, ExprPtr id, int l);
};

// Break statement 跳出语句
struct BreakStmt : Stmt
{
    std::vector<StmtPtr> body; // 跳出时执行的语句块
    BreakStmt(std::vector<StmtPtr> b, int l);
};

// Continue statement 继续语句
struct ContinueStmt : Stmt
{
    std::vector<StmtPtr> body; // 继续时执行的语句块
    ContinueStmt(std::vector<StmtPtr> b, int l);
};