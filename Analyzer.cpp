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
                    cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                         << ": '" << t->token.lexeme << "' does not declared in this scope" << endl;
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
                    cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                         << ": '" << t->token.lexeme << "' does not declared in this scope" << endl;
                }
            }
            if (info.type != "Array")
            {
                cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                     << ": " << t->token.lexeme << " does not an Array type" << endl;
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
                    cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                         << ": class " << currentClassName << " haven't a member function '" << functionName << "()'" << endl;
                    break;
                }
                SymbolTable::Kind currentFunctionKind = symbolTable->classesTableFind(currentClassName, currentFunctionName).kind;
                SymbolTable::Kind calledFunctionKind = symbolTable->classesTableFind(currentClassName, functionName).kind;
                // 再检查当前子过程和被调用过程是否都是method
                if (currentFunctionKind == SymbolTable::FUNCTION && calledFunctionKind == SymbolTable::FUNCTION)
                {
                    cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                         << ": subroutine " << functionName << " called as a method from within a function" << endl;
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
                        cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                             << ": class " << callerName << " hasn't a member function '" << functionName << "()'" << endl;
                        break;
                    }
                    if (info.kind == SymbolTable::METHOD)
                    {
                        cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                             << ": '" << functionName << "' is not a function in class " << callerName << endl;
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
                    // 先检查当前子过程是不是method
                    if (symbolTable->classesTableFind(currentClassName, currentFunctionName).kind == SymbolTable::FUNCTION);
                    {
                        cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                            << ": a method cannot called in a function" << endl;
                        break;
                    }
                    // 再检查caller有没有被声明
                    SymbolTable::Info objInfo = symbolTable->subroutineTableFind(callerName);
                    if (objInfo == SymbolTable::None)
                    {
                        objInfo = symbolTable->classesTableFind(currentClassName, callerName);
                        if (objInfo == SymbolTable::None)
                        {
                            cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                                 << ": '" << callerName << "' was not declared in this scope" << endl;
                            break;
                        }
                    }
                    // 再检查function
                    SymbolTable::Info functionInfo = symbolTable->classesTableFind(objInfo.type, functionName);
                    if (functionInfo == SymbolTable::None)
                    {
                        cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                             << ": class " << callerName << " haven't a member function " << functionName << "'()" << endl;
                        break;
                    }
                    if (functionInfo.kind != SymbolTable::METHOD)
                    {
                        cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                             << ": '" << functionName << "' is not a method in class " << callerName << endl;
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
        SymbolTable::Info info = symbolTable->subroutineTableFind("this");
        if (t->child[0] == nullptr && info.type != "void")
        {
                cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                     << ": return-statement with no value, in function returning '" << info.type << "'" << endl;
                break;
        }
        else if (t->child[0] != nullptr && info.type == "void")
        {
            cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                << ": return-statement with a value, in function returning void" << endl;
            break;
        }
        if (info.kind == SymbolTable::CONSTRUCTOR && t->child[0]->token.lexeme != "this")
        {
            cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
                << ": The return type of a constructor must be of the class type" << endl;
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
        cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
             << ": too few arguments to function " << functionName << "()" << endl;
        return;
    }
    else if (argumentSize > parameter.size())
    {
        cerr << "Error in class " << currentClassName << " in line " << t->token.currentRow
             << ": too many arguments to function " << functionName << endl;
        return;
    }
}

void Analyzer::check()
{
    buildClassesTable(tree);
    symbolTable->printClassesTable();
    checkStatements(tree);
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
