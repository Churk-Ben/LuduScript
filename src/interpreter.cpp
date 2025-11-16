#include "interpreter.h"
#include <stdexcept>
#include <algorithm>

void use(Expr e)
{
    (void)e;
}

// Value implementation
Value Value::makeInt(ll i)
{
    Value v;
    v.type = Type::NUM;
    v.nval = static_cast<double>(i);
    v.isInteger = true;
    return v;
}

Value Value::makeNum(double n)
{
    Value v;
    v.type = Type::NUM;
    v.nval = n;
    v.isInteger = false;
    return v;
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
    if (type == Type::NUM)
    {
        if (isInteger)
            return std::to_string(static_cast<ll>(nval));
        else
            return std::to_string(nval);
    }
    if (type == Type::BOOL)
        return bval ? "true" : "false";
    return "";
}

double Value::toNum() const
{
    if (type == Type::NUM)
        return nval;
    if (type == Type::STR)
    {
        try
        {
            return std::stod(sval);
        }
        catch (...)
        {
            return 0.0;
        }
    }
    if (type == Type::BOOL)
        return bval ? 1.0 : 0.0;
    return 0.0;
}

ll Value::toInt() const
{
    if (type == Type::NUM)
        return static_cast<ll>(nval);
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
    if (type == Type::NUM)
        return nval != 0.0;
    if (type == Type::STR)
        return !sval.empty();
    return false;
}

bool Value::isInt() const
{
    return type == Type::NUM && isInteger;
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
    if (lit->kind == LiteralExpr::Kind::INTEGER)
        return Value::makeInt(lit->ival);
    if (lit->kind == LiteralExpr::Kind::FLOAT)
        return Value::makeNum(lit->dval);
    if (lit->kind == LiteralExpr::Kind::STRING)
        return Value::makeStr(lit->sval);
    if (lit->kind == LiteralExpr::Kind::BOOL)
        return Value::makeBool(lit->bval);

    // 默认返回值，不应该到达这里
    throw std::runtime_error("Unknown literal kind");
}

Value Interpreter::evalIdent(IdentExpr *id)
{
    // First try to get variable from environment
    auto val = env.getVar(id->name);
    if (val.has_value())
        return *val;

    // If in object context, try to get field value from current object
    if (env.current_object.has_value() && env.declared_fields.find(id->name) != env.declared_fields.end())
    {
        auto &obj = *env.current_object;
        if (obj.contains(id->name))
        {
            auto &field = obj[id->name];
            if (field.is_number())
            {
                double val = field.get<double>();
                if (val == std::floor(val))
                    return Value::makeInt(static_cast<ll>(val));
                else
                    return Value::makeNum(val);
            }
            else if (field.is_string())
                return Value::makeStr(field.get<std::string>());
            else if (field.is_boolean())
                return Value::makeBool(field.get<bool>());
        }
    }

    // If in object context and variable not found, treat as field name
    if (env.current_object.has_value() && env.declared_fields.find(id->name) == env.declared_fields.end())
    {
        return Value::makeStr(id->name);
    }

    throw std::runtime_error("Undefined variable: " + id->name);
}

Value Interpreter::evalUnary(UnaryExpr *u)
{
    Value r = evalExpr(u->rhs.get());
    if (u->op == "!")
        return Value::makeBool(!r.toBool());
    if (u->op == "-")
        return Value::makeNum(-r.toNum());
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
        // If both are integers, return integer
        if (L.type == Value::Type::NUM && R.type == Value::Type::NUM && L.isInteger && R.isInteger)
        {
            return Value::makeInt(static_cast<ll>(L.nval) + static_cast<ll>(R.nval));
        }
        // Otherwise return float
        return Value::makeNum(L.toNum() + R.toNum());
    }
    if (op == "-")
    {
        // If both are integers, return integer
        if (L.type == Value::Type::NUM && R.type == Value::Type::NUM && L.isInteger && R.isInteger)
        {
            return Value::makeInt(static_cast<ll>(L.nval) - static_cast<ll>(R.nval));
        }
        // Otherwise return float
        return Value::makeNum(L.toNum() - R.toNum());
    }
    if (op == "*")
    {
        // If both are integers, return integer
        if (L.type == Value::Type::NUM && R.type == Value::Type::NUM && L.isInteger && R.isInteger)
        {
            return Value::makeInt(static_cast<ll>(L.nval) * static_cast<ll>(R.nval));
        }
        // Otherwise return float
        return Value::makeNum(L.toNum() * R.toNum());
    }
    if (op == "/")
    {
        // Division always returns float to handle fractional results
        double r = R.toNum();
        if (r == 0.0)
            throw std::runtime_error("Division by zero");
        return Value::makeNum(L.toNum() / r);
    }
    if (op == "%")
    {
        ll r = R.toInt();
        if (r == 0)
            throw std::runtime_error("Modulo by zero");
        return Value::makeNum(static_cast<double>(L.toInt() % r));
    }
    if (op == "==")
    {
        if (L.type == R.type)
        {
            if (L.type == Value::Type::NUM)
                return Value::makeBool(L.nval == R.nval);
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
            if (L.type == Value::Type::NUM)
                return Value::makeBool(L.nval != R.nval);
            if (L.type == Value::Type::STR)
                return Value::makeBool(L.sval != R.sval);
            if (L.type == Value::Type::BOOL)
                return Value::makeBool(L.bval != R.bval);
        }
        return Value::makeBool(true);
    }
    if (op == "<")
        return Value::makeBool(L.toNum() < R.toNum());
    if (op == ">")
        return Value::makeBool(L.toNum() > R.toNum());
    if (op == "<=")
        return Value::makeBool(L.toNum() <= R.toNum());
    if (op == ">=")
        return Value::makeBool(L.toNum() >= R.toNum());
    if (op == "&&")
        return Value::makeBool(L.toBool() && R.toBool());
    if (op == "||")
        return Value::makeBool(L.toBool() || R.toBool());

    throw std::runtime_error("Unknown binary operator: " + op);
}

Value Interpreter::evalCall(CallExpr *c)
{
    // 目前解释器还不支持函数调用
    use(*c);
    throw std::runtime_error("Function calls not supported");
}

Value Interpreter::evalAccess(AccessExpr *a)
{
    // 目前解释器还不支持成员访问
    use(*a);
    throw std::runtime_error("Member access not supported");
}
