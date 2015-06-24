#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include "Scanner.h"
#include "Parser.h"
#include <unordered_map>
#include <deque>
#include <string>
#include <vector>

using namespace std;

class SymbolTable
{
public:
	enum TableKind
	{ 
		Function, 
		Variable 
	};
	struct VariableTable
	{
		Scanner::TokenType type;
		bool isArray;
	};
	struct FunctionTable
	{
		Scanner::TokenType type;
		vector<VariableTable> params;
	};
	using FunctionTableType = unordered_map<std::string, FunctionTable> ;
	using VariableTableType = unordered_map<std::string, VariableTable> ;
	/* 函数表 */
	FunctionTableType functionTable;
	/* 变量表栈, 变量表被保存在栈里面, 用来实现作用域规则 */
	deque<VariableTableType> variablaTable;
	bool insert(TableKind kind, Parser::TreeNode *t, bool isArray = false);
	void insertParam(std::string funcName, Parser::TreeNode *t, bool isArray = false);
	pair<FunctionTable, bool> findFunctionTable(std::string);
	pair<VariableTable, bool> findVariableTable(std::string);
	SymbolTable()
	{
		functionTable.clear();
		variablaTable.resize(1);
	}
};

#endif