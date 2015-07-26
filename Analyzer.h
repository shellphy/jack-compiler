#ifndef _ANALYZER_H
#define _ANALYZER_H

#include "Parser.h"
#include "SymbolTable.h"
#include <map>
#include <vector>

class Analyzer
{
private:
    Parser::TreeNode *tree;
    SymbolTable symbolTable;
    
    void buildClassesTable(Parser::TreeNode *t);
    void checkStatements(Parser::TreeNode *t);
    void checkStatement(Parser::TreeNode *t);
    string checkExpression(Parser::TreeNode *t);
    bool checkCaller(SymbolTable::Info & objInfo, Parser::TreeNode *t);
    bool checkFunction(SymbolTable::Info & objInfo, SymbolTable::Info & functionInfo, Parser::TreeNode *t);
    bool checkArguments(SymbolTable::Info & objInfo, SymbolTable::Info & functionInfo, Parser::TreeNode *t, string functionName);
public:
    Analyzer(Parser::TreeNode *t);
    void check();
};

#endif