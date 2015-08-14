#include "Analyzer.h"
#include <iostream>

using namespace std;

Analyzer::Analyzer(Parser::TreeNode *t)
{
    symbolTable = SymbolTable::getInstance();
    tree = t;
}

// 遍历表达式树
void Analyzer::checkExpression(Parser::TreeNode *t)
{
    if (t != nullptr)
    {
        for (int i = 0; i < 5; i++)
            checkExpression(t->child[i]);
        switch (t->nodeKind)
        {
        case Parser::VAR_K:
        {
            SymbolTable::Info info = symbolTable->subroutineTableFind(t->token.lexeme);
            if (info == SymbolTable::None)
            {
                info = symbolTable->classesTableFind(currentClassName, t->token.lexeme);
                if (info == SymbolTable::None)
                {
                    error5(currentClassName, t->token.row, t->token.lexeme);
                }
            }
        }
        break;
        case Parser::ARRAY_K:
        {
            SymbolTable::Info info = symbolTable->subroutineTableFind(t->token.lexeme);
            if (info == SymbolTable::None)
            {
                info = symbolTable->classesTableFind(currentClassName, t->token.lexeme);
                if (info == SymbolTable::None)
                {
                    error5(currentClassName, t->token.row, t->token.lexeme);
                }
            }
            if (info.type != "Array")
            {
                error6(currentClassName, t->token.row, t->token.lexeme);
            }
        }
        break;
        case Parser::CALL_EXPRESSION_K:
        case Parser::CALL_STATEMENT_K:
        {
            if (t->token.lexeme.find('.') == string::npos)     // call_statement -> ID ( expressions ) 
            {
                // 先检查函数有没有在当前类中声明
                string functionName = t->token.lexeme;
                if (symbolTable->classesTableFind(currentClassName, functionName) == SymbolTable::None)
                {
                    error7(currentClassName, currentClassName, t->token.row, functionName);
                    break;
                }
                SymbolTable::Kind currentFunctionKind = symbolTable->classesTableFind(currentClassName, currentFunctionName).kind;
                SymbolTable::Kind calledFunctionKind = symbolTable->classesTableFind(currentClassName, functionName).kind;
                // 再检查当前子过程和被调用过程是否都是method
                if (currentFunctionKind == SymbolTable::FUNCTION && calledFunctionKind == SymbolTable::FUNCTION)
                {
                    error8(currentClassName, t->token.row, functionName);
                    break;
                }
                // 再检查函数的参数是否正确
                SymbolTable::Info info = symbolTable->classesTableFind(currentClassName, functionName);
                checkArguments(t, info.args, functionName);
                t->child[0]->nodeKind = Parser::METHOD_CALL_K;
            }
            else                                            // call_statement -> ID . ID ( expressions ) 
            {
                // 先检查caller
                string callerName = Parser::getCallerName(t->token.lexeme);
                string functionName = Parser::getFunctionName(t->token.lexeme);
                if (symbolTable->classIndexFind(callerName) == true)    // 如果caller是类
                {
                    // 再检查function
                    SymbolTable::Info info = symbolTable->classesTableFind(callerName, functionName);
                    if (info == SymbolTable::None)
                    {
                        error7(currentClassName, callerName, t->token.row, functionName);
                        break;
                    }
                    if (info.kind == SymbolTable::METHOD)
                    {
                        error9(currentClassName, callerName, t->token.row, functionName);
                        break;
                    }
                    // 再检查参数
                    checkArguments(t, info.args, functionName);
                    if (info.kind == SymbolTable::FUNCTION)
                        t->child[0]->nodeKind = Parser::FUNCTION_CALL_K;
                    else if (info.kind == SymbolTable::CONSTRUCTOR)
                        t->child[0]->nodeKind = Parser::CONSTRUCTOR_CALL_K;
                }
                else                                                   // 如果调用者是对象
                {
                    // 再检查caller有没有被声明
                    SymbolTable::Info objInfo = symbolTable->subroutineTableFind(callerName);
                    if (objInfo == SymbolTable::None)
                    {
                        objInfo = symbolTable->classesTableFind(currentClassName, callerName);
                        if (objInfo == SymbolTable::None)
                        {
                            error5(currentClassName, t->token.row, callerName);
                            break;
                        }
                    }
                    // 再检查function
                    SymbolTable::Info functionInfo = symbolTable->classesTableFind(objInfo.type, functionName);
                    if (functionInfo == SymbolTable::None)
                    {
                        error7(currentClassName, callerName, t->token.row, functionName);
                        break;
                    }
                    if (functionInfo.kind != SymbolTable::METHOD)
                    {
                        error10(currentClassName, callerName, t->token.row, functionName);
                        break;
                    }
                    // 再检查参数
                    checkArguments(t, functionInfo.args, functionName);
                    t->child[0]->nodeKind = Parser::METHOD_CALL_K;
//                    t->token.lexeme = objInfo.type + "." + functionName;
                }
            }
            break;
        }
        }
    }
}

/*
  检查赋值语句, if语句, while语句, return语句, 函数调用语句
*/
void Analyzer::checkStatement(Parser::TreeNode *t)
{
    switch (t->nodeKind)
    {
    case Parser::CLASS_K:
        currentClassName = t->child[0]->token.lexeme;
        break;
    case Parser::ASSIGN_K:
    {
        checkExpression(t->child[0]);
        checkExpression(t->child[1]);
    }
    case Parser::IF_STATEMENT_K:
    case Parser::WHILE_STATEMENT_K:
    {
        checkExpression(t->child[0]);
        break;
    }
    case Parser::RETURN_STATEMENT_K:
    {
        checkExpression(t->child[0]);
        SymbolTable::Info info = symbolTable->subroutineTableFind("this");
        if (t->child[0] == nullptr && info.type != "void")
        {
            error11(currentClassName, info.type, t->token.row);
            break;
        }
        else if (t->child[0] != nullptr && info.type == "void")
        {
            error12(currentClassName, t->token.row);
            break;
        }
        if (info.kind == SymbolTable::CONSTRUCTOR && t->child[0]->token.lexeme != "this")
        {
            error13(currentClassName, t->token.row);
            break;
        }
        break;
    }
    case Parser::CALL_STATEMENT_K:
        checkExpression(t);
        break;
    }
}

void Analyzer::checkArguments(Parser::TreeNode *t, vector<string> parameter, string functionName)
{
    int argumentSize = 0;
    for (auto p = t->child[0]->next; p != nullptr; p = p->next)
    {
        checkExpression(p);
        argumentSize++;
    }
    if (argumentSize < parameter.size())
    {
        error14(currentClassName, functionName, t->token.row);
        return;
    }
    else if (argumentSize > parameter.size())
    {
        error15(currentClassName, functionName, t->token.row);
        return;
    }
}

void Analyzer::check()
{
    buildClassesTable(tree);
//    symbolTable->printClassesTable();
    checkMain();
    checkStatements(tree);
}

void Analyzer::checkMain()
{
    if (symbolTable->classIndexFind("Main") == false)
    {
        error16();
        return;
    }
    auto info = symbolTable->classesTableFind("Main", "main");
    if (info == SymbolTable::None)
    {
        error17();
        return;
    }
    if (info.kind != SymbolTable::FUNCTION)
    {
        error18();
        return;
    }
    if (info.type != "void")
    {
        error19();
        return;
    }
    if (info.args.size() > 0)
    {
        error20();
        return;
    }
}

void Analyzer::buildClassesTable(Parser::TreeNode *t)
{
    static int depth = 0;
    if (depth > 2)
        return;
    while (t != nullptr)
    {
        symbolTable->classesTableInsert(t);
        for (int i = 0; i < 5; i++)
        {
            depth++;
            buildClassesTable(t->child[i]);
            depth--;
        }
        t = t->next;
    }
}

void Analyzer::checkStatements(Parser::TreeNode *t)
{
    while (t != nullptr)
    {
        symbolTable->subroutineTableInsert(t);
        checkStatement(t);
        for (int i = 0; i < 5; i++)
            checkStatements(t->child[i]);
        t = t->next;
    }
}
