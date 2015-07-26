#include "SymbolTable.h"
#include <iostream>

using namespace std;

SymbolTable::SymbolTable()
{
    currentClassNumber = 0;
}

string SymbolTable::getFunctionName(string name)
{
    auto iter = name.cbegin();
    while (iter != name.cend())
    {
        if (*iter == ':')
            break;
        ++iter;
    }
    string name(name.cbegin(), iter);
}

string SymbolTable::getClassName(string name)
{
    auto iter = name.cbegin();
    while (iter != name.cend())
    {
        if (*iter == ':')
            break;
        ++iter;
    }
    ++iter;
    string name(iter, name.cend());
}

void SymbolTable::classesTableInsert(Parser::TreeNode *t)
{
    if (t->nodeKind == Parser::CLASS_K)
    {
        map<string, Info> temp;
        classesTable.push_back(temp);
        string name = t->child[0]->token.lexeme;
        classIndex.insert({name, classesTable.size() - 1});
    }
    else if (t->nodeKind == Parser::CLASS_VAR_DEC_K)
    {
        Info info;
        if (t->child[0]->token.lexeme == "field")
            info.kind = FIELD;
        else if (t->child[0]->token.lexeme == "static")
            info.kind = STATIC;
        info.type = t->child[1]->token.lexeme;
        for (auto p = t->child[2]; p != nullptr; p = p->next)
        {
            string name = p->token.lexeme;
            if (classesTable.back().insert({ name, info }).second == false)   // 插入失败,符号表中有已经存在的元素
                if (t->nodeKind == Parser::CLASS_VAR_DEC_K)
                    cerr << "Error in line " << p->token.currentRow <<
                    ": redeclaration of '" << info.type << " " << name << "'" << endl;
        }
    }
    else if (t->nodeKind == Parser::SUBROUTINE_DEC_K)
    {
        Info info;
        if (t->child[0]->token.lexeme == "function")
            info.kind = FIELD;
        else if (t->child[0]->token.lexeme == "method")
            info.kind = STATIC;
        else if (t->child[0]->token.lexeme == "constructor")
            info.kind = CONSTRUCTOR;
        info.type = t->child[1]->token.lexeme;
        for (auto iter = t->child[3]; iter != nullptr; ++iter)
        {
            string type = iter->child[0]->token.lexeme;
            info.args.push_back(type);
        }
        string name = getFunctionName(t->child[2]->token.lexeme);
        if (classesTable.back().insert({ name, info }).second == false)
            cerr << "Error in line " << t->child[2]->token.currentRow <<
            "redeclaration of '" << info.type << " " << name << "()" << endl;

    }
}

void SymbolTable::subroutineTableInsert(Parser::TreeNode *t)
{
    if (t->nodeKind == Parser::SUBROUTINE_DEC_K)
    {
        initialSubroutineTable();
        string className = getClassName(t->child[2]->token.lexeme);
        string functionName = getFunctionName(t->child[2]->token.lexeme);
        currentClassNumber = classIndex.find(className)->second;
        Info info = classesTable[currentClassNumber].find(functionName)->second;
        subroutineTable["this"] = info;
    }
    else if (t->nodeKind == Parser::PARAM_K)
    {
        Info info;
        info.kind = ARG;
        info.type = t->child[0]->token.lexeme;
        string name = t->child[1]->token.lexeme;
        auto iter = classesTable[currentClassNumber].find(name);
        if (iter != classesTable[currentClassNumber].end())
        {
            cerr << "Error in line " << t->child[1]->token.currentRow <<
                ": redeclaration of '" << info.type << " " << name << "'" << endl;
            return;
        }
        if (subroutineTable.insert({ name, info }).second == false)
        {
            cerr << "Error in line " << t->child[1]->token.currentRow
                << ": redifinition of '" << info.type << " " << name << "'" << endl;
        }
    }
    else if (t->nodeKind == Parser::VAR_DEC_K)
    {
        Info info;
        info.kind = VAR;
        info.type = t->child[0]->token.lexeme;
        for (auto p = t->child[1]; p != nullptr; p = p->next)
        {
            string name = p->token.lexeme;
            auto iter = classesTable[currentClassNumber].find(name);
            if (iter != classesTable[currentClassNumber].end())
            {
                cerr << "Error in line " << p->token.currentRow <<
                    ": redeclaration of '" << info.type << " " << name << "'" << endl;
                continue;
            }
            if (subroutineTable.insert({ name, info }).second == false)
            {
                cerr << "Error in line " << p->token.currentRow <<
                    ": redeclaration of '" << info.type << " " << name << "'" << endl;
            }
        }
    }
}

SymbolTable::Info SymbolTable::find(string name)
{
    auto sub_iter = subroutineTable.find(name);
    if (sub_iter == subroutineTable.end())
    {
        auto class_iter = classesTable[currentClassNumber].find(name);
        if (class_iter == classesTable[currentClassNumber].end())
            return None;
        else
            return class_iter->second;
    }
    else
        return sub_iter->second;
}

SymbolTable::Info SymbolTable::classesTableFind(string className, string functionName)
{
    int classTableNumber = classIndex.find(className)->second;
    auto iter = classesTable[classTableNumber].find(functionName);
    if (iter == classesTable[classTableNumber].end)
        return None;
    else
        return iter->second;
}
