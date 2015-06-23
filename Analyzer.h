#ifndef _ANALYZER_H
#define _ANALYZER_H

#include "Parser.h"
#include "SymbolTable.h"
#include <string>
#include <iostream>

using namespace std;

class Analyzer
{
private:
	SymbolTable symbolTable;
	void error(Scanner::Token token, string s);
	void traverse(Parser::TreeNode *tree);
	bool typeCompatible(Parser::TreeNode *t1, Parser::TreeNode *t2);
	bool paramCompatible(Parser::TreeNode *t);
public:
	void checkType(Parser::TreeNode *tree);
};

#endif