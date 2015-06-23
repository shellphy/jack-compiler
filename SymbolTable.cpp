#include "SymbolTable.h"
#include "Parser.h"

void SymbolTable::insertParam(string funcName, Parser::TreeNode *t, bool isArray)
{

}

bool SymbolTable::insert(TableKind kind, Parser::TreeNode *t, bool isArray)
{
	if (kind == Function)
	{
		FunctionTable val;
		val.type = t->child[1]->token.kind;
		auto ret = functionTable.insert({ t->child[2]->token.lexeme, val });
		return ret.second;
	}
	else if (kind == Variable)
	{
		VariableTable val;
		val.type = t->child[0]->token.kind;
		val.isArray = isArray;
		auto ret = variablaTable.back().insert({ t->child[1]->token.lexeme, val });
		return ret.second;
	}
}

bool SymbolTable::find(TableKind kind, string s)
{
	if (kind == Function)
	{
		if (functionTable.find(s) == functionTable.end())
			return false;
		else
			return true;
	}
	else if (kind == Variable)
	{
		for (auto it = variablaTable.rbegin(); it != variablaTable.rend(); ++it)
		{
			if (it->find(s) != it->end())
				return true;
		}
		return false;
	}
}
