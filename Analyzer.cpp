#include "Analyzer.h"
#include "Parser.h"
#include "SymbolTable.h"

void Analyzer::error(Scanner::Token token, string s)
{
	cout << "error: (" << token.currentRow << ", " << token.currentColumn << ")  ";
	cout << s << endl;
}

void Analyzer::traverse( Parser::TreeNode *tree)
{
	for (int i = 0; i < 5; i++)
		checkType(tree->child[i]);
}

bool Analyzer::typeCompatible(Parser::TreeNode *t1, Parser::TreeNode *t2)
{
	return true;
}

bool Analyzer::paramCompatible(Parser::TreeNode *t)
{
	return true;
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
			if (tree->child[2] != nullptr)
			{
				if (symbolTable.insert(SymbolTable::TableKind::Variable, tree, true) == false)
					error(tree->child[1]->token, "变量重复声明");
				symbolTable.insertParam(functionName, tree, true);
				checkType(tree->next);
			}
			else if (tree->child[2] == nullptr)
			{
				if (symbolTable.insert(SymbolTable::TableKind::Variable, tree) == false)
					error(tree->child[1]->token, "变量重复声明");
				symbolTable.insertParam(functionName, tree);
				checkType(tree->next);
			}
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
			if (typeCompatible(tree->child[0], tree->child[1]) == false)
				error(tree->child[0]->token, "类型不兼容");
			return;
		}
		case Parser::Call_kind:
		{
			if (symbolTable.find(SymbolTable::TableKind::Function, tree->child[0]->token.lexeme) == false)
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
				error(tree->token, "break不能出现在这里");
			return;
		}
		case Parser::Continue_kind:
		{
			if (inWhile == false)
				error(tree->token, "continue不能出现在这里");
			return;
		}
		case Parser::Operation_kind:
		{
			if (typeCompatible(tree->child[0], tree->child[1]) == false)
				error(tree->child[0]->token, "类型不兼容");
			return;
		}
		case Parser::Bool_kind:
		{
			return;
		}
		case Parser::Identifier_kind:
		{
			if (symbolTable.find(SymbolTable::TableKind::Variable, tree->token.lexeme) == false)
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