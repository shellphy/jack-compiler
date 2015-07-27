#include "SymbolTable.h"
#include <iostream>

using namespace std;

SymbolTable::Info SymbolTable::None;

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
    return string(++iter, name.cend());
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
    return string(name.cbegin(), iter);
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
                    cerr << "Error in class " << currentClass << " in line " <<  p->token.currentRow <<
                    ": redeclaration of '" << info.type << " " << name << "'" << endl;
        }
    }
    else if (t->nodeKind == Parser::SUBROUTINE_DEC_K)
    {
        Info info;
        if (t->child[0]->token.lexeme == "function")
            info.kind = FUNCTION;
        else if (t->child[0]->token.lexeme == "method")
            info.kind = METHOD;
        else if (t->child[0]->token.lexeme == "constructor")
            info.kind = CONSTRUCTOR;
        info.type = t->child[1]->token.lexeme;
        for (auto p = t->child[3]; p != nullptr; p = p->next)
        {
            string type = p->child[0]->token.lexeme;
            info.args.push_back(type);
        }
        string name = getFunctionName(t->child[2]->token.lexeme);
        if (classesTable.back().insert({ name, info }).second == false)
            cerr << "Error in class " << currentClass << " in line " << t->child[2]->token.currentRow <<
            ": redeclaration of '" << info.type << " " << name << "()" << endl;

    }
}

void SymbolTable::subroutineTableInsert(Parser::TreeNode *t)
{
    if (t->nodeKind == Parser::CLASS_K)
        currentClass = t->child[0]->token.lexeme;
    else if (t->nodeKind == Parser::SUBROUTINE_DEC_K)
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
        if (info.type != "int" && info.type != "float" && info.type != "char" && 
            info.type != "void" && info.type != "string" && info.type != "boolean")
        {
            if (classIndex.find(info.type) == classIndex.end())
            {
                cerr << "Error in class " << currentClass << " in line " << t->child[1]->token.currentRow <<
                    ": '" << info.type << "' not declaraed" << endl;
                return;
            }
        }
        string name = t->child[1]->token.lexeme;
        auto iter = classesTable[currentClassNumber].find(name);
        if (iter != classesTable[currentClassNumber].end())
        {
            cerr << "Error in class " << currentClass << " in line " << t->child[1]->token.currentRow <<
                ": redeclaration of '" << info.type << " " << name << "'" << endl;
            return;
        }
        if (subroutineTable.insert({ name, info }).second == false)
        {
            cerr << "Error in class " << currentClass << " in line " << t->child[1]->token.currentRow
                << ": redifinition of '" << info.type << " " << name << "'" << endl;
            return;
        }
    }
    else if (t->nodeKind == Parser::VAR_DEC_K)
    {
        Info info;
        info.kind = VAR;
        info.type = t->child[0]->token.lexeme;
        if (info.type != "int" && info.type != "float" && info.type != "char" &&
            info.type != "void" && info.type != "string" && info.type != "boolean")
        {
            if (classIndex.find(info.type) == classIndex.end())
            {
                cerr << "Error in class " << currentClass << " in line " << t->child[1]->token.currentRow <<
                    ": '" << info.type << "' not declaraed" << endl;
                return;
            }
        }

        for (auto p = t->child[1]; p != nullptr; p = p->next)
        {
            string name = p->token.lexeme;
            auto iter = classesTable[currentClassNumber].find(name);
            if (iter != classesTable[currentClassNumber].end())
            {
                cerr << "Error in class " << currentClass << " in line " << p->token.currentRow <<
                    ": redeclaration of '" << info.type << " " << name << "'" << endl;
                continue;
            }
            if (subroutineTable.insert({ name, info }).second == false)
            {
                cerr << "Error in class " << currentClass << " in line " << p->token.currentRow <<
                    ": redeclaration of '" << info.type << " " << name << "'" << endl;
            }
        }
    }
}

SymbolTable::Info SymbolTable::subroutineTableFind(string name)
{
    auto iter = subroutineTable.find(name);
    if (iter == subroutineTable.end())
        return None;
    else
        return iter->second;
}

SymbolTable::Info SymbolTable::classesTableFind(string className, string functionName)
{
    int classTableNumber = classIndex.find(className)->second;
    auto iter = classesTable[classTableNumber].find(functionName);
    if (iter == classesTable[classTableNumber].end())
        return None;
    else
        return iter->second;
}

void SymbolTable::initialSubroutineTable()
{
    subroutineTable.clear();
}

void SymbolTable::printClassesTable()
{
    cout << "class index: " << endl;
    cout << "类名\t\t编号\t\t" << endl;
    for (auto iter = classIndex.cbegin(); iter != classIndex.cend(); ++iter)       
        cout << iter->first << "\t\t" << iter->second << endl;
    cout << endl;
    cout << "********************符号表********************" << endl;
    for (int i = 0; i < classesTable.size(); i++)
    {
        cout << "class table1: " << endl;
        cout << "name\ttype\tkind\tvars" << endl;
        for (auto iter = classesTable[i].cbegin(); iter != classesTable[i].cend(); ++iter)
        {
            cout << iter->first << "\t" << iter->second.type << "\t" << iter->second.kind << "\t";
            for (int k = 0; k < iter->second.args.size(); ++k)
                cout << iter->second.args[k] << "\t";
            cout << endl;
        }
        cout << endl << endl;
    }

}

bool SymbolTable::classIndexFind(string className)
{
    if (classIndex.find(className) == classIndex.end())
        return false;
    else
        return true;
}
