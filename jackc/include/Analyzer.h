#ifndef _ANALYZER_H
#define _ANALYZER_H

#include "Error.h"
#include "Parser.h"
#include "SymbolTable.h"
#include <vector>

class Analyzer {
private:
  Parser::TreeNode *tree;
  SymbolTable *symbolTable;
  string currentClassName;    // 遍历树的时候, 保存当前类的名称
  string currentFunctionName; // 遍历树的时候, 保存当前函数的名称
  void buildClassesTable(Parser::TreeNode *t);
  void checkStatements(Parser::TreeNode *t);
  void checkStatement(Parser::TreeNode *t);
  void checkExpression(Parser::TreeNode *t);
  void checkArguments(Parser::TreeNode *t, vector<string> const& parameter,
                      string const& functionName);
  void checkMain();

public:
  Analyzer(Parser::TreeNode *t);
  void check();
};

#endif
