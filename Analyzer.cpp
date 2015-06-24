#include "Analyzer.h"
#include "Parser.h"
#include "SymbolTable.h"

void Analyzer::error(Scanner::Token token, string s)
{
	cout << "error: (" << token.currentRow << ", " << token.currentColumn << ")  "
		<< token.lexeme << " " << s << endl;
}

void Analyzer::traverse( Parser::TreeNode *tree)
{
	for (int i = 0; i < 5; i++)
		checkType(tree->child[i]);
}

Scanner::TokenType Analyzer::typeCompatible(Parser::TreeNode *tree)
{
	switch (tree->nodeKind)
	{
	case Parser::Operation_kind:
	case Parser::Assign_kind:
	{
		Scanner::TokenType t1 = typeCompatible(tree->child[0]);
		Scanner::TokenType t2 = typeCompatible(tree->child[1]);
		if (t1 != t2)
		{
			error(tree->child[0]->token, "类型不兼容");
			return t1;
		}
		else
			return t1;
	}
	case Parser::Identifier_kind:
	{
		auto t = symbolTable.findVariableTable(tree->token.lexeme);
		return t.first.type;
	}
	case Parser::Call_kind:
	{
		auto t = symbolTable.findFunctionTable(tree->child[0]->token.lexeme);
		return t.first.type;
	}
	case Parser::Const_kind:
	{
		return tree->token.kind;
	}
	default:
		break;
	}
}

bool Analyzer::paramCompatible(Parser::TreeNode *t)
{
	auto node = t->child[1];
	vector<SymbolTable::VariableTable> params = symbolTable.findFunctionTable(t->child[0]->token.lexeme).first.params;
	auto it = params.begin();
	while (node != nullptr && it != params.end())
	{
		auto type = typeCompatible(node);
		if (type != it->type)
			return false;
		++it;
	}
	if (it == params.end())
		return true;
	return false;
}

void Analyzer::checkType(Parser::TreeNode *tree)
{
	static string functionName;
	static bool inWhile = false;
	while (tree != nullptr)
	{
		switch (tree->nodeKind)
		{
		case Parser::Function_declaration_kind:
		{
			functionName = tree->child[1]->token.lexeme;
			symbolTable.insert(SymbolTable::TableKind::Function, tree);
			SymbolTable::VariableTableType temp;
			symbolTable.variablaTable.push_back(temp);
			checkType(tree->child[2]);
			checkType(tree->child[3]);
			symbolTable.variablaTable.pop_back();
			checkType(tree->next);

			return;
		}
		case Parser::compound_statement_kind:
		{			
			SymbolTable::VariableTableType temp;
			symbolTable.variablaTable.push_back(temp);
			traverse(tree);
			symbolTable.variablaTable.pop_back();
			checkType(tree->next);

			return;
		}
		case Parser::Var_declaration_kind:
		{
			if (symbolTable.insert(SymbolTable::TableKind::Variable, tree) == false)
				error(tree->child[1]->token, "变量重复声明");
			checkType(tree->next);

			return;
		}
		case Parser::Var_array_declaration_kind:
		{
			if (symbolTable.insert(SymbolTable::TableKind::Variable, tree, true) == false)
				error(tree->child[1]->token, "变量重复声明");
			checkType(tree->next);

			return;
		}
		case Parser::Const_kind:
		{
			return;
		}
		case Parser::Param_kind:
		{
			if (symbolTable.insert(SymbolTable::TableKind::Variable, tree, true) == false)
				error(tree->child[1]->token, "变量重复声明");
			symbolTable.insertParam(functionName, tree, true);
			checkType(tree->next);
		
			return;
		}
		case Parser::Param_Array_kind:
		{
			return;
		}
		case Parser::If_kind:
		{
			traverse(tree);
			checkType(tree->next);
			return;
		}
		case Parser::While_kind:
		{
			inWhile = true;
			traverse(tree);
			inWhile = false;
			checkType(tree->next);
			return;
		}
		case Parser::Assign_kind:
		{
			typeCompatible(tree);
			checkType(tree->next);
			return;
		}
		case Parser::Call_kind:
		{
			if (symbolTable.findFunctionTable(tree->child[0]->token.lexeme).second == false)
				error(tree->child[0]->token, "函数" + tree->child[0]->token.lexeme + "未定义");
			if (paramCompatible(tree) == false)
				error(tree->child[1]->token, "函数实参与形参不匹配");
			checkType(tree->child[1]);
			checkType(tree->next);
			return;
		}
		case Parser::Return_kind:
		{
			traverse(tree);
			checkType(tree->next);
			return;
		}
		case Parser::Break_kind:
		{
			if (inWhile == false)
				error(tree->token, "不能出现在这里");
			checkType(tree->next);
			return;
		}
		case Parser::Continue_kind:
		{
			if (inWhile == false)
				error(tree->token, "不能出现在这里");
			checkType(tree->next);
			return;
		}
		case Parser::Operation_kind:
		{
			typeCompatible(tree);
			return;
		}
		case Parser::Bool_kind:
		{
			return;
		}
		case Parser::Identifier_kind:
		{
			if (symbolTable.findVariableTable(tree->token.lexeme).second == false)
				error(tree->token, "变量" + tree->token.lexeme + "未定义");
			return;
		}
		case Parser::Type_kind:
		{
			return;
		}
		default:
			return;
		}
	}
}