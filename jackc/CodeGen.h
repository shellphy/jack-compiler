#ifndef _CODE_GEN_H
#define _CODE_GEN_H

#include "Parser.h"
#include "SymbolTable.h"
#include <string>
#include <fstream>

using namespace std;

class CodeGen
{
private:
    enum Segment
    {
        CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP
    };
    enum Command
    {
        ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT
    };
    SymbolTable *symbolTable;
    Parser::TreeNode * tree;
    string currentClassName;
    string currentFunctionName;
    int ifLabelCount;
    int whileLabelCount;
    ofstream fout;
    bool isMethod;
    void writePush(Segment segment, int index);
    void writePop(Segment segment, int index);
    void writeArithmetic(Command command);
    void writeLabel(string label);
    void writeGoto(string label);
    void writeIf(string label);
    void writeCall(string name, int nArgs);
    void writeFunction(string name, int nArgs);
    void writeReturn();
    void writeExpression(Parser::TreeNode * t);
    void writeCallExpression(Parser::TreeNode * t);
    void translateCall(Parser::TreeNode *t);
public:
    CodeGen();
    void translate(Parser::TreeNode *t);
    void write(Parser::TreeNode *t);
};

#endif
