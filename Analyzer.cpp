#include "Analyzer.h"
#include <iostream>

using namespace std;

Analyzer::Analyzer(Parser::TreeNode *t)
{
    tree = t;
}

string Analyzer::checkExpression(Parser::TreeNode *t)
{
    switch (t->nodeKind)
    {
    case Parser::INT_CONST_K:
        return "int";
    case Parser::CHAR_CONST_K:
        return "char";
    case Parser::FLOAT_CONST_K:
        return "float";
    case Parser::STRING_CONST_K:
        return "string";
    case Parser::BOOL_CONST_K:
        return "boolean";
    case Parser::THIS_K:
        return symbolTable.find("this").type;
    case Parser::VAR_K:
    {
        if (t->child[0] != nullptr)
        {
            string type = checkExpression(t->child[0]);
            if (type != "int")
                cerr << "Error in line " << t->token.currentRow
                << " size of array '" << t->token.lexeme << "' has non-integer type '" << type << "'" << endl;
        }
        return symbolTable.find(t->token.lexeme).type;
    }
    case Parser::LOGICAL_EXPRESSION_K:
        if (t->child[1] == nullptr)
        {
            string type = checkExpression(t->child[0]);
            if (type != "boolean")
                cerr << "Error in line " << t->token.currentRow
                << " there must be a int expression" << endl;
            return "int";
        }
        string type1 = checkExpression(t->child[0]);
        string type2 = checkExpression(t->child[1]);
        if (type1 != "boolean")
            cerr << "Error in line " << t->token.currentRow
            << " could not convert '" << type1 << "' to a boolean type" << endl;
        if (type2 != "boolean")
            cerr << "Error in line " << t->token.currentRow
            << " could not convert '" << type2 << "' to a boolean type" << endl;
        return "int";
    case Parser::BOOL_EXPRESSION_K:
        string type1 = checkExpression(t->child[0]);
        string type2 = checkExpression(t->child[1]);
        if (type1 != "int" || type2 != "int")
            cerr << "Error in line " << t->token.currentRow
            << " invalid operands of types '" << type1 << " and '" << type2 << "' to binary 'operator" << t->token.lexeme << "'" << endl;
        return type1;
    case Parser::COMPARE_K:
        string type1 = checkExpression(t->child[0]);
        string type2 = checkExpression(t->child[1]);
        if (type1 != type2)
            cerr << "Error in line " << t->token.currentRow
            << " invalid comparison between '" << type1 << "' and '" << type2 << "'" << endl;
        return "boolean";
    case Parser::OPERATION_K:
        string type1 = checkExpression(t->child[0]);
        string type2 = checkExpression(t->child[1]);
        if (type1 != type2)
            cerr << "Error in line " << t->token.currentRow
            << " invalid operands of types '" << type1 << "' and '" << type2 << "' to binary 'opeartor" << t->token.lexeme << "'" << endl;
        return t->token.lexeme;
    }
}

/*
  检查赋值语句, if语句, while语句, return语句, 函数调用语句
*/
void Analyzer::checkStatement(Parser::TreeNode *t)
{
    switch (t->nodeKind)
    {
    case Parser::ASSIGN_K:
    {
        string varName = t->child[0]->token.lexeme;
        SymbolTable::Info info = symbolTable.find(varName);
        if (info == SymbolTable::None)
        {
            cerr << "'" << varName << "' was not declared in this scope" << endl;
            break;
        }
        string type = checkExpression(t->child[1]);
        if (info.type != type)
            cerr << "Error in line " << t->child[0]->token.currentRow << ", type mismatch" << endl;
    }
    case Parser::IF_STATEMENT_K:
    case Parser::WHILE_STATEMENT_K:
    {
        string type = checkExpression(t->child[0]);
        if (type == "string" || type == "float")
            cerr << "could not convert '" << type << "' to bool expression" << endl;
    }
    case Parser::RETURN_STATEMENT_K:
    {
        SymbolTable::Info info = symbolTable.find("this");
        if (t->child[0] == nullptr)
        {
            if (info.type != "void")
                cerr << "Error in line " << t->token.currentRow
                << ", return-statement with no value, in function returning '" << info.type << "'" << endl;
        }
        else
        {
            string type = checkExpression(t->child[0]);
            if (info.type != type)
                cerr << "Error in line " << t->token.currentRow
                << ", cannot convert '" << type << "' to " << info.type << endl;
        }

    }
    case Parser::CALL_STATEMENT_K:
    {
        checkExpression(t);

        //if (t->child[2] == nullptr)   // call_statement -> ID ( expressions ) 
        //{
        //    SymbolTable::Info objInfo = symbolTable.find("this");
        //    SymbolTable::Info functionInfo;
        //    if (checkFunction(objInfo, functionInfo, t->child[0]) == false)
        //        break;
        //    if (checkArguments(objInfo, functionInfo, t->child[1], t->child[0]->token.lexeme) == false)
        //        break;
        //}
        //else                          // call_statement -> ID . ID ( expressions ) 
        //{
        //    SymbolTable::Info objInfo;
        //    SymbolTable::Info functionInfo;
        //    if (checkCaller(objInfo, t->child[0]) == false)
        //        break;
        //    if (checkFunction(objInfo, functionInfo, t->child[1]) == false)
        //        break;
        //    if (checkArguments(objInfo, functionInfo, t->child[2], t->child[1]->token.lexeme) == false)
        //        break;
        //}
    }
    }
}

bool Analyzer::checkCaller(SymbolTable::Info & objInfo, Parser::TreeNode *t)
{
    string objName = t->token.lexeme;
    objInfo = symbolTable.find(objName);
    if (objInfo == SymbolTable::None)
    {
        cerr << "Error in line " << t->token.currentRow
            << " '" << objName << "' was not declared in this scope" << endl;
        return false;
    }
    if (objInfo.type == "int" || objInfo.type == "char" || objInfo.type == "string"
        || objInfo.type == "boolean" || objInfo.type == "void" || objInfo.type == "float")
    {
        cerr << "Error in line " << t->token.currentRow
            << " " << objName << " is noa a class" << endl;
        return false;
    }
    return true;
}

bool Analyzer::checkFunction(SymbolTable::Info & objInfo, SymbolTable::Info & functionInfo, Parser::TreeNode *t)
{
    string functionName = t->token.lexeme;
    functionInfo = symbolTable.classesTableFind(objInfo.type, functionName);
    if (functionInfo == SymbolTable::None)
    {
        cerr << "Error in line " << t->token.lexeme
            << " class " << objInfo.type << " haven't member " << functionName << "()" << endl;
        return false;
    }
    if (functionInfo.kind != SymbolTable::FUNCTION || functionInfo.kind != SymbolTable::METHOD || functionInfo.kind != SymbolTable::CONSTRUCTOR)
    {
        cerr << "Error in line " << t->token.lexeme
            << " " << functionName << " was not a function or method or constructor" << endl;
        return false;
    }
    return true;
}

bool Analyzer::checkArguments(SymbolTable::Info & objInfo, SymbolTable::Info & functionInfo, Parser::TreeNode *t, string functionName)
{
    vector<string> args;
    for (auto p = t; p != nullptr; p = p->next)
    {
        string type = checkExpression(p);
        args.push_back(type);
    }
    if (args.size() < functionInfo.args.size())
    {
        cerr << "Error in line " << t->child[0]->token.currentRow
            << " too few arguments to function " << functionName << endl;
        return false;
    }
    else if (args.size() > functionInfo.args.size())
    {
        cerr << "Error in line " << t->child[0]->token.currentRow
            << " too many arguments to function " << functionName << endl;
        return false;
    }
    else if (args.size() == functionInfo.args.size())
    {
        for (int i = 0; i < args.size(); i++)
        {
            if (args[i] != functionInfo.args[i])
            {
                cerr << "Error in line " << t->child[0]->token.currentRow
                    << " connot convert " << args[i] << " to " << functionInfo.args[i] << endl;
                break;
            }
        }
    }

}


void Analyzer::check()
{
    buildClassesTable(tree);
    checkStatements(tree);
}

/*
需要修改为循环式的层序遍历
*/
void Analyzer::buildClassesTable(Parser::TreeNode *t)
{
    while (t != nullptr)
    {
        symbolTable.classesTableInsert(t);
        for (int i = 0; i < 5; i++)
            buildClassesTable(t);
        t = t->next;
    }
}

void Analyzer::checkStatements(Parser::TreeNode *t)
{
    while (t != nullptr)
    {
        checkStatement(t);
        for (int i = 0; i < 5; i++)
            checkStatements(t);
        t = t->next;
    }
}
