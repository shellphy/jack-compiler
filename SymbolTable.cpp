#include "SymbolTable.h"
#include "Parser.h"

void SymbolTable::insertParam(string funcName, Parser::TreeNode *t, bool isArray)
{
	auto func_iter = functionTable.find(funcName);
	auto val = findVariableTable(t->child[1]->token.lexeme).first;
	func_iter->second.params.push_back(val);
}

bool SymbolTable::insert(TableKind kind, Parser::TreeNode *t, bool isArray)
{
	if (kind == Function)
	{
		FunctionTable val;
		val.type = t->child[0]->token.kind;
		Scanner::TokenType type;
		if (val.type == Scanner::RW_INT)
			val.type = Scanner::INT;
		else if (val.type == Scanner::RW_FLOAT)
			val.type = Scanner::FLOAT;
		else if (val.type = Scanner::RW_BOOL)
			val.type = Scanner::BOOL;
		else if (val.type == Scanner::RW_STRING)
			val.type = Scanner::STRING;
		else if (val.type == Scanner::RW_CHAR)
			val.type = Scanner::CHAR;
		auto ret = functionTable.insert({ t->child[1]->token.lexeme, val });
		return ret.second;
	}
	else if (kind == Variable)
	{
		VariableTable val;
		val.type = t->child[0]->token.kind;
		if (val.type == Scanner::RW_INT)
			val.type = Scanner::INT;
		else if (val.type == Scanner::RW_FLOAT)
			val.type = Scanner::FLOAT;
		else if (val.type = Scanner::RW_BOOL)
			val.type = Scanner::BOOL;
		else if (val.type == Scanner::RW_STRING)
			val.type = Scanner::STRING;
		else if (val.type == Scanner::RW_CHAR)
			val.type = Scanner::CHAR;
		val.isArray = isArray;
		auto ret = variablaTable.back().insert({ t->child[1]->token.lexeme, val });
		return ret.second;
	}
}

pair<SymbolTable::FunctionTable, bool> SymbolTable::findFunctionTable(std::string s)
{
	pair<FunctionTable, bool> rt;
	auto func_iter = functionTable.find(s);
	if (func_iter == functionTable.end())
	{
		rt.second = false;
		return rt;
	}
	else
	{
		rt.first = func_iter->second;
		rt.second = true;
		return rt;
	}
}

pair<SymbolTable::VariableTable, bool> SymbolTable::findVariableTable(std::string s)
{
	pair<SymbolTable::VariableTable, bool> rt;
	for (auto it = variablaTable.rbegin(); it != variablaTable.rend(); ++it)
	{
		auto var_iter = it->find(s);
		if (var_iter != it->end())
		{
			rt.first = var_iter->second;
			rt.second = true;
			return rt;
		}
	}
	rt.second = false;
	return rt;
}
