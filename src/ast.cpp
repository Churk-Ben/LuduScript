#include "ast.h"

// LiteralExpr constructors
LiteralExpr::LiteralExpr(ll v, int l) : Expr(l), kind(Kind::INTEGER), ival(v) {}
LiteralExpr::LiteralExpr(double d, int l) : Expr(l), kind(Kind::FLOAT), dval(d) {}
LiteralExpr::LiteralExpr(std::string s, int l) : Expr(l), kind(Kind::STRING), sval(std::move(s)) {}
LiteralExpr::LiteralExpr(bool b, int l) : Expr(l), kind(Kind::BOOL), bval(b) {}

// IdentExpr constructor
IdentExpr::IdentExpr(std::string n, int l) : Expr(l), name(std::move(n)) {}

// UnaryExpr constructor
UnaryExpr::UnaryExpr(std::string o, ExprPtr r, int l) : Expr(l), op(std::move(o)), rhs(std::move(r)) {}

// BinaryExpr constructor
BinaryExpr::BinaryExpr(ExprPtr l, std::string o, ExprPtr r, int ln) : Expr(ln), op(std::move(o)), lhs(std::move(l)), rhs(std::move(r)) {}

// CallExpr constructor
CallExpr::CallExpr(ExprPtr c, std::vector<ExprPtr> a, int l) : Expr(l), callee(std::move(c)), args(std::move(a)) {}

// AccessExpr constructor
AccessExpr::AccessExpr(ExprPtr t, std::string m, int l) : Expr(l), target(std::move(t)), member(std::move(m)) {}

// Program constructor
Program::Program() : Node(1) {}

// ExprStmt constructor
ExprStmt::ExprStmt(ExprPtr e, int l) : Stmt(l), expr(std::move(e)) {}

// AssignStmt constructor
AssignStmt::AssignStmt(std::string n, ExprPtr e, int l) : Stmt(l), name(std::move(n)), expr(std::move(e)) {}

// DeclStmt constructors
DeclStmt::DeclStmt(std::string t, std::string n, std::optional<ExprPtr> i, int l) : Stmt(l), type(std::move(t)), name(std::move(n)), init(std::move(i)) {}
DeclStmt::DeclStmt(std::string t, std::string n, std::vector<StmtPtr> block, int l) : Stmt(l), type(std::move(t)), name(std::move(n)), initBlock(std::move(block)) {}

// IfStmt constructor
IfStmt::IfStmt(ExprPtr c, int l) : Stmt(l), cond(std::move(c)) {}

// ForStmt constructor
ForStmt::ForStmt(std::string it, int l) : Stmt(l), iter(std::move(it)) {}

// ObjStmt constructor
ObjStmt::ObjStmt(std::string c, ExprPtr id, int l) : Stmt(l), className(std::move(c)), idExpr(std::move(id)) {}

BreakStmt::BreakStmt(std::vector<StmtPtr> b, int l) : Stmt(l), body(std::move(b)) {}

ContinueStmt::ContinueStmt(std::vector<StmtPtr> b, int l) : Stmt(l), body(std::move(b)) {}