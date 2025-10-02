#include "interpreter.h"
#include <stdexcept>

void Interpreter::execStmt(Stmt *s)
{
    if (auto es = dynamic_cast<ExprStmt *>(s))
    {
        // Evaluate and ignore
        try
        {
            evalExpr(es->expr.get());
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(std::string("Runtime error (line ") + std::to_string(es->line) + "): " + ex.what());
        }
        return;
    }

    if (auto as = dynamic_cast<AssignStmt *>(s))
    {
        Value v = evalExpr(as->expr.get());

        // Check if variable exists in any outer scope first
        bool found = false;
        for (int i = int(env.stack.size()) - 1; i >= 0; --i)
        {
            auto it = env.stack[i].find(as->name);
            if (it != env.stack[i].end())
            {
                // Update existing variable in its original scope
                env.stack[i][as->name] = v;
                found = true;
                break;
            }
        }

        if (!found)
        {
            // Variable doesn't exist in stack, check if inside object
            if (env.current_object.has_value())
            {
                // Check if it's already declared as object field
                if (env.declared_fields.count(as->name) > 0 || env.current_object->contains(as->name))
                {
                    // Update existing object field
                    if (v.type == Value::Type::NUM && v.isInteger)
                        env.current_object->operator[](as->name) = json(static_cast<ll>(v.nval));
                    else if (v.type == Value::Type::NUM)
                        env.current_object->operator[](as->name) = json(v.nval);
                    else if (v.type == Value::Type::BOOL)
                        env.current_object->operator[](as->name) = json(v.bval);
                    else
                        env.current_object->operator[](as->name) = json(v.sval);
                }
                else
                {
                    // Create new object field
                    if (v.type == Value::Type::NUM && v.isInteger)
                        env.current_object->operator[](as->name) = json(static_cast<ll>(v.nval));
                    else if (v.type == Value::Type::NUM)
                        env.current_object->operator[](as->name) = json(v.nval);
                    else if (v.type == Value::Type::BOOL)
                        env.current_object->operator[](as->name) = json(v.bval);
                    else
                        env.current_object->operator[](as->name) = json(v.sval);
                    env.declared_fields.insert(as->name);
                }
            }
            else
            {
                // Create in current scope
                env.setVar(as->name, v);
            }
        }
        return;
    }

    if (auto ds = dynamic_cast<DeclStmt *>(s))
    {
        Value v;
        if (!ds->initBlock.empty())
        {
            // Create new scope for the initialization block
            env.pushScope();

            // Keep object context active so fields can be accessed in initialization blocks
            // This is required by SYNTAX.md specification

            // Execute statements in the block and track the last expression value
            Value lastExprValue;
            bool hasLastExpr = false;
            std::string lastVar;

            for (size_t i = 0; i < ds->initBlock.size(); ++i)
            {
                auto &stmt = ds->initBlock[i];
                bool isLastStmt = (i == ds->initBlock.size() - 1);

                // Check if this is an expression statement (the last expression should be returned)
                if (auto exprStmt = dynamic_cast<ExprStmt *>(stmt.get()))
                {
                    lastExprValue = evalExpr(exprStmt->expr.get());
                    hasLastExpr = true;
                }
                // Special handling for if statements that can return values
                else if (isLastStmt)
                {
                    if (auto ifStmt = dynamic_cast<IfStmt *>(stmt.get()))
                    {
                        // For if statements as the last statement, we need to capture their return value
                        lastExprValue = execIfWithReturn(ifStmt);
                        hasLastExpr = true;
                    }
                    else
                    {
                        execStmt(stmt.get());
                        // If it's a declaration, track it as potential last variable
                        if (auto innerDecl = dynamic_cast<DeclStmt *>(stmt.get()))
                        {
                            lastVar = innerDecl->name;
                        }
                    }
                }
                else
                {
                    execStmt(stmt.get());
                    // If it's a declaration, track it as potential last variable
                    if (auto innerDecl = dynamic_cast<DeclStmt *>(stmt.get()))
                    {
                        lastVar = innerDecl->name;
                    }
                }
            }

            // Get the final value: prefer last expression, then last declared variable
            Value blockResult;
            bool hasResult = false;

            if (hasLastExpr)
            {
                blockResult = lastExprValue;
                hasResult = true;
            }
            else if (!lastVar.empty())
            {
                auto varValue = env.getVar(lastVar);
                if (varValue.has_value())
                {
                    blockResult = varValue.value();
                    hasResult = true;
                }
            }

            // Pop the scope
            env.popScope();

            // Set the final value
            if (hasResult)
            {
                v = blockResult;
            }
            else
            {
                // No valid result, use default
                if (ds->type == "num")
                    v = Value::makeNum(0.0);
                else if (ds->type == "str")
                    v = Value::makeStr("");
                else if (ds->type == "bool")
                    v = Value::makeBool(false);
            }
        }
        else if (ds->init.has_value())
        {
            v = evalExpr(ds->init->get());
        }
        else
        {
            // Default values
            if (ds->type == "num")
                v = Value::makeNum(0.0);
            else if (ds->type == "str")
                v = Value::makeStr("");
            else if (ds->type == "bool")
                v = Value::makeBool(false);
        }

        // If inside object, write to object field, else to var
        if (env.current_object.has_value())
        {
            if (v.type == Value::Type::NUM && v.isInteger)
                env.current_object->operator[](ds->name) = json(static_cast<ll>(v.nval));
            else if (v.type == Value::Type::NUM)
                env.current_object->operator[](ds->name) = json(v.nval);
            else if (v.type == Value::Type::BOOL)
                env.current_object->operator[](ds->name) = json(v.bval);
            else
                env.current_object->operator[](ds->name) = json(v.sval);
            env.declared_fields.insert(ds->name);
        }
        else
        {
            env.setVar(ds->name, v);
        }
        return;
    }

    if (auto is = dynamic_cast<IfStmt *>(s))
    {
        Value cond = evalExpr(is->cond.get());
        if (cond.toBool())
        {
            execBlock(is->thenBody);
        }
        else
        {
            // Check elif conditions
            bool executed = false;
            for (auto &elif : is->elifs)
            {
                Value elifCond = evalExpr(elif.first.get());
                if (elifCond.toBool())
                {
                    execBlock(elif.second);
                    executed = true;
                    break;
                }
            }

            // Execute else block if no elif was executed
            if (!executed && !is->elseBody.empty())
            {
                execBlock(is->elseBody);
            }
        }
        return;
    }

    if (auto fs = dynamic_cast<ForStmt *>(s))
    {
        ll start = 1, end = 1, step = 1;

        if (fs->args.size() == 1)
        {
            // for(i, N) -> i from 1 to N
            end = evalExpr(fs->args[0].get()).toInt();
        }
        else if (fs->args.size() == 2)
        {
            // for(i, start, end) -> i from start to end
            start = evalExpr(fs->args[0].get()).toInt();
            end = evalExpr(fs->args[1].get()).toInt();
        }
        else if (fs->args.size() == 3)
        {
            // for(i, start, end, step)
            start = evalExpr(fs->args[0].get()).toInt();
            end = evalExpr(fs->args[1].get()).toInt();
            step = evalExpr(fs->args[2].get()).toInt();
        }

        // Iterate
        env.pushScope();
        if (step == 0)
            step = 1;

        if (step > 0)
        {
            for (ll it = start; it <= end; it += step)
            {
                env.setVar(fs->iter, Value::makeInt(it));
                try
                {
                    // Execute statements directly without creating additional scope
                    for (auto &st : fs->body)
                        execStmt(st.get());
                }
                catch (const BreakException &)
                {
                    break;
                }
                catch (const ContinueException &)
                {
                    continue;
                }
                catch (...)
                {
                    env.popScope();
                    throw;
                }
            }
        }
        else
        {
            for (ll it = start; it >= end; it += step)
            {
                env.setVar(fs->iter, Value::makeInt(it));
                try
                {
                    // Execute statements directly without creating additional scope
                    for (auto &st : fs->body)
                        execStmt(st.get());
                }
                catch (const BreakException &)
                {
                    break;
                }
                catch (const ContinueException &)
                {
                    continue;
                }
                catch (...)
                {
                    env.popScope();
                    throw;
                }
            }
        }
        env.popScope();
        return;
    }

    if (auto os = dynamic_cast<ObjStmt *>(s))
    {
        // Create object
        env.current_object = json::object();
        env.declared_fields.clear();
        env.current_object->operator[]("class") = os->className;

        Value idv = evalExpr(os->idExpr.get());
        // ID as int if int, num if num, else string
        if (idv.type == Value::Type::NUM)
        {
            if (idv.isInteger)
            {
                env.current_object->operator[]("id") = static_cast<ll>(idv.nval);
            }
            else
            {
                env.current_object->operator[]("id") = idv.nval;
            }
        }
        else
        {
            env.current_object->operator[]("id") = idv.toStr();
        }

        // Execute body with object context; use new scope for body variables
        env.pushScope();
        for (auto &st : os->body)
        {
            execStmt(st.get());
        }
        env.popScope();

        // Push to output
        env.output.push_back(*env.current_object);
        env.current_object.reset();
        env.declared_fields.clear();
        return;
    }

    if (auto bs = dynamic_cast<BreakStmt *>(s))
    {
        // 执行break语句块中的语句
        for (const auto &stmt : bs->body)
        {
            execStmt(stmt.get());
        }
        throw BreakException();
    }

    if (auto cs = dynamic_cast<ContinueStmt *>(s))
    {
        // 执行continue语句块中的语句
        for (const auto &stmt : cs->body)
        {
            execStmt(stmt.get());
        }
        throw ContinueException();
    }

    throw std::runtime_error("Unknown statement node");
}

// Helper function to execute if statement and return its value
Value Interpreter::execIfWithReturn(IfStmt *is)
{
    Value cond = evalExpr(is->cond.get());
    if (cond.toBool())
    {
        return execBlockWithReturn(is->thenBody);
    }
    else
    {
        // Check elif conditions
        for (auto &elif : is->elifs)
        {
            Value elifCond = evalExpr(elif.first.get());
            if (elifCond.toBool())
            {
                return execBlockWithReturn(elif.second);
            }
        }

        // Execute else block if available
        if (!is->elseBody.empty())
        {
            return execBlockWithReturn(is->elseBody);
        }
    }

    // No matching condition, return default value
    return Value::makeNum(0.0);
}

// Helper function to execute block and return the last expression value
Value Interpreter::execBlockWithReturn(const std::vector<StmtPtr> &body)
{
    env.pushScope();

    Value lastValue = Value::makeNum(0.0);
    bool hasValue = false;

    for (size_t i = 0; i < body.size(); ++i)
    {
        auto &stmt = body[i];
        bool isLastStmt = (i == body.size() - 1);

        if (isLastStmt)
        {
            if (auto exprStmt = dynamic_cast<ExprStmt *>(stmt.get()))
            {
                // Last statement is an expression, return its value
                lastValue = evalExpr(exprStmt->expr.get());
                hasValue = true;
            }
            else
            {
                execStmt(stmt.get());
            }
        }
        else
        {
            execStmt(stmt.get());
        }
    }

    env.popScope();

    return hasValue ? lastValue : Value::makeNum(0.0);
}

void Interpreter::execBlock(const std::vector<StmtPtr> &body)
{
    env.pushScope();
    for (auto &st : body)
        execStmt(st.get());
    env.popScope();
}