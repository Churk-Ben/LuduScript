#include "interpreter.h"
#include <stdexcept>
#include <algorithm>

// Value implementation
Value Value::makeInt(ll i)
{
    Value x;
    x.type = Type::INT;
    x.ival = i;
    return x;
}

Value Value::makeStr(std::string s)
{
    Value x;
    x.type = Type::STR;
    x.sval = std::move(s);
    return x;
}

Value Value::makeBool(bool b)
{
    Value x;
    x.type = Type::BOOL;
    x.bval = b;
    return x;
}

std::string Value::toStr() const
{
    if (type == Type::STR)
        return sval;
    if (type == Type::INT)
        return std::to_string(ival);
    if (type == Type::BOOL)
        return bval ? "true" : "false";
    return "";
}

ll Value::toInt() const
{
    if (type == Type::INT)
        return ival;
    if (type == Type::STR)
    {
        try
        {
            return std::stoll(sval);
        }
        catch (...)
        {
            return 0;
        }
    }
    if (type == Type::BOOL)
        return bval ? 1 : 0;
    return 0;
}

bool Value::toBool() const
{
    if (type == Type::BOOL)
        return bval;
    if (type == Type::INT)
        return ival != 0;
    if (type == Type::STR)
        return !sval.empty();
    return false;
}

// Env implementation
void Env::pushScope()
{
    stack.emplace_back();
}

void Env::popScope()
{
    if (!stack.empty())
        stack.pop_back();
}

void Env::setVar(const std::string &k, const Value &v)
{
    if (stack.empty())
        pushScope();
    stack.back()[k] = v;
}

std::optional<Value> Env::getVar(const std::string &k)
{
    for (int i = int(stack.size()) - 1; i >= 0; --i)
    {
        auto it = stack[i].find(k);
        if (it != stack[i].end())
            return it->second;
    }
    return std::nullopt;
}

// Interpreter implementation
void Interpreter::execute(Program *program)
{
    for (auto &stmt : program->stmts)
    {
        execStmt(stmt.get());
    }
}

std::string Interpreter::getOutput(bool pretty) const
{
    if (pretty)
        return env.output.dump(2);
    else
        return env.output.dump();
}

Value Interpreter::evalExpr(Expr *e)
{
    if (auto lit = dynamic_cast<LiteralExpr *>(e))
        return evalLiteral(lit);
    if (auto id = dynamic_cast<IdentExpr *>(e))
        return evalIdent(id);
    if (auto u = dynamic_cast<UnaryExpr *>(e))
        return evalUnary(u);
    if (auto b = dynamic_cast<BinaryExpr *>(e))
        return evalBinary(b);
    if (auto c = dynamic_cast<CallExpr *>(e))
        return evalCall(c);
    if (auto a = dynamic_cast<AccessExpr *>(e))
        return evalAccess(a);
    throw std::runtime_error("Unknown expression node");
}

Value Interpreter::evalLiteral(LiteralExpr *lit)
{
    if (lit->kind == LiteralExpr::Kind::NUMBER)
        return Value::makeInt(lit->ival);
    if (lit->kind == LiteralExpr::Kind::STRING)
        return Value::makeStr(lit->sval);
    return Value::makeBool(lit->bval);
}

Value Interpreter::evalIdent(IdentExpr *id)
{
    // Check if it's an undeclared field in object context
    if (env.current_object.has_value() && env.declared_fields.find(id->name) == env.declared_fields.end())
    {
        return Value::makeStr(id->name);
    }
    
    auto val = env.getVar(id->name);
    if (val.has_value())
        return *val;
    
    throw std::runtime_error("Undefined variable: " + id->name);
}

Value Interpreter::evalUnary(UnaryExpr *u)
{
    Value r = evalExpr(u->rhs.get());
    if (u->op == "!")
        return Value::makeBool(!r.toBool());
    if (u->op == "-")
        return Value::makeInt(-r.toInt());
    throw std::runtime_error("Unknown unary operator: " + u->op);
}

Value Interpreter::evalBinary(BinaryExpr *b)
{
    Value L = evalExpr(b->lhs.get());
    Value R = evalExpr(b->rhs.get());
    const std::string &op = b->op;
    
    if (op == "+")
    {
        // If either is string, do string concat
        if (L.type == Value::Type::STR || R.type == Value::Type::STR)
            return Value::makeStr(L.toStr() + R.toStr());
        return Value::makeInt(L.toInt() + R.toInt());
    }
    if (op == "-")
        return Value::makeInt(L.toInt() - R.toInt());
    if (op == "*")
        return Value::makeInt(L.toInt() * R.toInt());
    if (op == "/")
    {
        ll r = R.toInt();
        if (r == 0)
            throw std::runtime_error("Division by zero");
        return Value::makeInt(L.toInt() / r);
    }
    if (op == "%")
    {
        ll r = R.toInt();
        if (r == 0)
            throw std::runtime_error("Modulo by zero");
        return Value::makeInt(L.toInt() % r);
    }
    if (op == "==")
    {
        if (L.type == R.type)
        {
            if (L.type == Value::Type::INT)
                return Value::makeBool(L.ival == R.ival);
            if (L.type == Value::Type::STR)
                return Value::makeBool(L.sval == R.sval);
            if (L.type == Value::Type::BOOL)
                return Value::makeBool(L.bval == R.bval);
        }
        return Value::makeBool(false);
    }
    if (op == "!=")
    {
        if (L.type == R.type)
        {
            if (L.type == Value::Type::INT)
                return Value::makeBool(L.ival != R.ival);
            if (L.type == Value::Type::STR)
                return Value::makeBool(L.sval != R.sval);
            if (L.type == Value::Type::BOOL)
                return Value::makeBool(L.bval != R.bval);
        }
        return Value::makeBool(true);
    }
    if (op == "<")
        return Value::makeBool(L.toInt() < R.toInt());
    if (op == ">")
        return Value::makeBool(L.toInt() > R.toInt());
    if (op == "<=")
        return Value::makeBool(L.toInt() <= R.toInt());
    if (op == ">=")
        return Value::makeBool(L.toInt() >= R.toInt());
    if (op == "&&")
        return Value::makeBool(L.toBool() && R.toBool());
    if (op == "||")
        return Value::makeBool(L.toBool() || R.toBool());
    
    throw std::runtime_error("Unknown binary operator: " + op);
}

Value Interpreter::evalCall(CallExpr *c)
{
    // For now, we don't support function calls in this simple interpreter
    throw std::runtime_error("Function calls not supported");
}

Value Interpreter::evalAccess(AccessExpr *a)
{
    // For now, we don't support member access in this simple interpreter
    throw std::runtime_error("Member access not supported");
}