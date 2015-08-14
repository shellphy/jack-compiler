#include "SymbolTable.h"
#include <iostream>
#include <cassert>
#include "Error.h"

using namespace std;

SymbolTable::Info SymbolTable::None;

SymbolTable * SymbolTable::instance = nullptr;

SymbolTable * SymbolTable::getInstance()
{
    if (instance == nullptr)
        instance = new SymbolTable();
    return instance;
}

SymbolTable::SymbolTable()
{
    currentClassNumber = 0;
    static_index = 0;
    field_index = 0;
    arg_index = 0;
    var_index = 0;
    errorNum = 0;
}


void SymbolTable::classesTableInsert(Parser::TreeNode *t)
{
    if (t->nodeKind == Parser::CLASS_K)
    {
        map<string, Info> temp;
        classesTable.push_back(temp);
        currentClass = t->child[0]->token.lexeme;
        int index = classesTable.size() - 1;
        classIndex.insert({currentClass, index});
        static_index = field_index = 0;
    }
    else if (t->nodeKind == Parser::CLASS_VAR_DEC_K)            // t = CLASS_VAR_DEC_K  
    {                                                           // t->child[0] = static | field
        Info info;                                              // t->child[1] = type          
        info.type = t->child[1]->token.lexeme;                  // t->child[2] = varName - varName - varName ...
        for (auto p = t->child[2]; p != nullptr; p = p->next)
        {
            string name = p->token.lexeme;
            if (t->child[0]->token.lexeme == "field")
            {
                info.kind = FIELD;
                info.index = field_index++;
            }
            else if (t->child[0]->token.lexeme == "static")
            {
                info.kind = STATIC;
                info.index = static_index++;
            }

            if (classesTable.back().insert({ name, info }).second == false)   // 插入失败,符号表中有已经存在的元素
            {
                error2(currentClass, p->token.row, info.type, name);
            }
        }
    }
    else if (t->nodeKind == Parser::SUBROUTINE_DEC_K)           // t = SUBROUTINE_DEC_K
    {                                                           // t->child[0] = function
        Info info;                                              // t->child[1] = type
        if (t->child[0]->token.lexeme == "function")            // t->child[2] = functionName
            info.kind = FUNCTION;                               // t->child[3] = arg - arg - arg ...
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
        string name = Parser::getFunctionName(t->child[2]->token.lexeme);
        if (classesTable.back().insert({ name, info }).second == false)
        {
            error3(currentClass, t->child[0]->token.row, info.type, name);
        }
    }
}

void SymbolTable::subroutineTableInsert(Parser::TreeNode *t)
{
    if (t->nodeKind == Parser::CLASS_K)
        currentClass = t->child[0]->token.lexeme;
    else if (t->nodeKind == Parser::SUBROUTINE_DEC_K)                               // t = SUBROUTINE_DEC_K
    {                                                                               // t->child[0] = function
        initialSubroutineTable();                                                   // t->child[1] = type
        string className = Parser::getCallerName(t->child[2]->token.lexeme);        // t->child[2] = functionName
        string functionName = Parser::getFunctionName(t->child[2]->token.lexeme);   // t->child[3] = arg - arg - arg ...
        currentClassNumber = classIndex.find(className)->second;
        Info info = classesTable[currentClassNumber].find(functionName)->second;
        subroutineTable["this"] = info;
        var_index = arg_index = 0;
    }
    else if (t->nodeKind == Parser::PARAM_K)        // t = PARAM_K
    {                                               // t->child[0] = type
        // 先检查type是否合理                       // t->child[1] = varName
        Info info;                                  
        info.kind = ARG;
        info.index = arg_index++;
        info.type = t->child[0]->token.lexeme;
        if (info.type != "int" && info.type != "char" && 
            info.type != "void" && info.type != "string" && info.type != "boolean")     // 如果不是基本类型
        {
            if (classIndexFind(info.type) == false)     // 也不是类类型
            {
                error4(currentClass, t->child[1]->token.row, info.type);
                return;
            }
        }
        // 再检查varName是否合理
        string varName = t->child[1]->token.lexeme;
        if (subroutineTable.insert({ varName, info }).second == false)
        {
            error2(currentClass, t->child[1]->token.row, info.type, varName);
            return;
        }
    }
    else if (t->nodeKind == Parser::VAR_DEC_K)          // t = VAR_DEC_K
    {                                                   // t->child[0] = type
        Info info;                                      // t->child[1] = varName - varName - varName
        info.kind = VAR;
        info.type = t->child[0]->token.lexeme;
        // 先检查type是否合理
        if (info.type != "int" && info.type != "char" &&
            info.type != "void" && info.type != "string" && info.type != "boolean")
        {
            if (classIndex.find(info.type) == classIndex.end())
            {
                errorNum++;
                error4(currentClass, t->child[1]->token.row, info.type);
                return;
            }
        }
        // 再检查varName是否合理
        for (auto p = t->child[1]; p != nullptr; p = p->next)
        {
            string varName = p->token.lexeme;
            info.index = var_index++;
            if (subroutineTable.insert({ varName, info }).second == false)
            {
                error2(currentClass, p->token.row, info.type, varName);
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
    assert(classIndexFind(className) == true);
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
        cout << "class table: " << i << endl;
        cout << "name\ttype\tkind\tvars" << endl;
        for (auto iter = classesTable[i].cbegin(); iter != classesTable[i].cend(); ++iter)
        {
            cout << iter->first << "\t" << iter->second.type << "\t" << iter->second.kind << "\t" << iter->second.index;
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

int SymbolTable::getFieldNumber(string className)
{
    assert(classIndexFind(className) == true);
    int classNum = classIndex.find(className)->second;
    int nField = 0;
    for (auto iter = classesTable[classNum].cbegin(); iter != classesTable[classNum].cend(); ++iter)
        if (iter->second.kind == FIELD)
            nField++;
    return nField;
}
