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
        int index;
        vector<string> args;
        Info()
        {
            type = "0";
            kind = NONE;
            index = 0;
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
    int static_index;
    int field_index;
    int arg_index;
    int var_index;
    int errorNum;
    map<string, int> classIndex;                // 从类名到数组索引
    vector<map<string, Info>> classesTable;     // 类符号表数组, 将一直保留着不会被销毁
    map<string, Info> subroutineTable;          // 函数符号表
    int currentClassNumber;     // 遍历语法树的时候, 保存当前类符号表数组索引
    string currentClass;        // 遍历语法树的时候, 保存当前类名称
    void initialSubroutineTable();          // 销毁函数符号表
    SymbolTable();
    static SymbolTable * instance;      // 指向符号表单实例对象
public:
    static SymbolTable * getInstance();     // 返回符号表单实例对象
    void classesTableInsert(Parser::TreeNode *t);       // 类符号表的插入操作
    void subroutineTableInsert(Parser::TreeNode *t);    // 函数符号表的插入操作
    
    Info subroutineTableFind(string name);  // 函数符号表的查找操作
    Info classesTableFind(string className, string functionName);   // 类符号表的查找操作
    bool classIndexFind(string className);  // 判断className是不是合法的类名
    
    int getFieldNumber(string className);
    void printClassesTable();       // 测试程序, 打印类符号表
};

#endif
