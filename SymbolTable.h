#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include "Parser.h"
#include <map>
#include <vector>

class SymbolTable
{
public:
    enum Kind
    {
        STATIC, FIELD, ARG, VAR, FUNCTION, METHOD, CONSTRUCTOR, NONE
    };
    class Info
    {
    public:
        string type;    // int, float, char, string
        Kind kind;      // kind : static, field, var, argument 
        vector<string> args;
        Info()
        {
            type = "0";
            kind = NONE;
        }
        friend bool operator==(Info info1, Info info2)
        {
            if (info1.type == info2.type && info1.kind == info2.kind && info1.args == info2.args)
                return true;
            else
                return false;
        }
    };
    static Info None;
private:
    vector<map<string, Info>> classesTable;     // 类符号表数组
    map<string, int> classIndex;                // 从类名到数组索引
    map<string, Info> subroutineTable;          // 函数符号表
    
    int currentClassNumber;     // 当前类符号表数组索引
    string currentClass;        // 当前类名称
    string getFunctionName(string name);
    string getClassName(string name);
    void initialSubroutineTable();
public:
    SymbolTable();
    void classesTableInsert(Parser::TreeNode *t);       // 类符号表的插入操作
    void subroutineTableInsert(Parser::TreeNode *t);    // 函数符号表的插入操作
    Info subroutineTableFind(string name);

//    Info find(string name);     
    Info classesTableFind(string className, string functionName);
    bool classIndexFind(string className);

    void printClassesTable();
};

#endif
