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
        // If inside object, write to object field, else to var
        if (env.current_object.has_value())
        {
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
        else
        {
            env.setVar(as->name, v);
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
            
            // Temporarily disable object context for block execution
            auto savedObject = env.current_object;
            env.current_object = std::nullopt;
            
            // Execute statements in the block and get the last declared variable
            std::string lastVar;
            for (auto& stmt : ds->initBlock)
            {
                execStmt(stmt.get());
                // Track the last declared variable
                if (auto innerDecl = dynamic_cast<DeclStmt*>(stmt.get()))
                {
                    lastVar = innerDecl->name;
                }
            }
            
            // Get the value of the last declared variable before popping scope
            Value blockResult;
            bool hasResult = false;
            
            if (!lastVar.empty())
            {
                auto varValue = env.getVar(lastVar);
                if (varValue.has_value())
                {
                    blockResult = varValue.value();
                    hasResult = true;
                }
            }
            
            // Restore object context and pop the scope
            env.current_object = savedObject;
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
                    execBlock(fs->body);
                }
                catch (const BreakException&)
                {
                    break;
                }
                catch (const ContinueException&)
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
                    execBlock(fs->body);
                }
                catch (const BreakException&)
                {
                    break;
                }
                catch (const ContinueException&)
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
        throw BreakException();
    }
    
    if (auto cs = dynamic_cast<ContinueStmt *>(s))
    {
        throw ContinueException();
    }
    
    throw std::runtime_error("Unknown statement node");
}

void Interpreter::execBlock(const std::vector<StmtPtr> &body)
{
    env.pushScope();
    for (auto &st : body)
        execStmt(st.get());
    env.popScope();
}