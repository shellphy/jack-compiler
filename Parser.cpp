#include "Parser.h"
#include "Scanner.h"
#include <string>
#include <iostream>
#include <deque>

using namespace std;


void Parser::print()
{
	printSyntaxTree(syntaxTree);
}

void Parser::printSyntaxTree(Parser::TreeNode *tree)
{
	static int indentno = 0;
	indentno += 2;
	while (tree != nullptr)
	{
		for (int i = 0; i < indentno; i++)
			cout << " ";
		switch (tree->nodeKind)
		{	
		case Parser::Var_declaration_kind:				
			cout << "var declaration" << endl;
			break;
		case Parser::Var_array_declaration_kind:		
			cout << "array declaratoin" << endl;			
			break;
		case Parser::Const_declaration_kind:			
			cout << "const declaration" << endl;			
			break;
		case Parser::Const_array_declaration_kind:		
			cout << "const array declaration" << endl;		
			break;
		case Parser::Function_declaration_kind:			
			cout << "function declaration" << endl;			
			break;
		case Parser::Param_kind:						
			cout << "param" << endl;			
			break;
		case Parser::Argument_kind:						
			cout << "argument" << endl;			
			break;
		case Parser::Object_kind:						
			cout << "object: " << tree->token.lexeme << endl;
			break;
		case Parser::If_kind:							
			cout << "if" << endl;		
			break;
		case Parser::While_kind:						
			cout << "while" << endl;	
			break;
		case Parser::Assign_kind:						
			cout << "==" << endl;
			break;
		case Parser::Call_kind:							
			cout << "function call" << endl;
			break;
		case Parser::Return_kind:						
			cout << "return" << endl;		
			break;
		case Parser::Operation_kind:					
			cout << tree->token.lexeme << endl;
			break;
		case Parser::Const_kind:						
			cout << "const: " << tree->token.lexeme << endl;
			break;
		case Parser::Identifier_kind:					
			cout << "ID: " << tree->token.lexeme << endl;
			break;
		case Parser::Type_kind:							
			cout << "type: " << tree->token.lexeme << endl;
			break;
		default:	
			break;
		}
		printSyntaxTree(tree->child[0]);
		printSyntaxTree(tree->child[1]);
		printSyntaxTree(tree->child[2]);
		printSyntaxTree(tree->child[3]);
		tree = tree->next;
	}
	indentno -= 2;
}

Parser::Parser(string filename) : scanner(filename)
{
	errorNumbers = 1;
}

void Parser::parse()
{
	scanner.scanToken();
	tokens = scanner.getTokens();
	syntaxTree = parse_program();
	cout << "**************************syntaxTree*****************************" << endl;
}

void Parser::syntaxError(string want, string got)
{
	cerr << "\n>>>error ("<< Parser::errorNumbers << ")" << ": Syntax error at position row " << currentToken.currentRow 
		 << ", column " << currentToken.currentColumn << endl;
	cerr << "\tunexpected token " << got << endl;
	Parser::errorNumbers++;
}

Parser::TreeNode * Parser::getSyntaxTree()
{
	return syntaxTree;
}

void Parser::readNextToken()
{
	if (!tokens->empty())
	{
		currentToken = tokens->front();
		tokens->pop_front();
	}
	else
		exit(-1);
}

void Parser::match(Scanner::TokenType expectedToken, string expectedString)
{
	if (currentToken.kind == expectedToken)
	{
		readNextToken();
	}
	else
	{
		syntaxError(expectedString, currentToken.lexeme);
		readNextToken();
	}
}

bool Parser::peek(Scanner::TokenType expectedToken)
{
	if (currentToken.kind == expectedToken)
	{
		readNextToken();
		return true;
	}
	else
		return false;
}

Parser::TreeNode * Parser::parse_program()
{
	TreeNode *t = parse_declaration_list();
	return t;
}

Parser::TreeNode * Parser::parse_declaration_list()
{
	TreeNode *t = nullptr;
	readNextToken();
	t = parse_declaration();
	TreeNode *p = t;
	while (currentToken.kind != Scanner::ENDOFFILE)
	{
		TreeNode *q = parse_declaration();
		if (q != nullptr)
		{
			if (t == nullptr)
				t = p = q;
			else
			{
				p->next = q;
				p = q;
			}
		}
	}
	return t;
}

Parser::TreeNode * Parser::parse_declaration()
{
	TreeNode *t = nullptr;
	if (!tokens->empty())
	{
		secondToken = tokens->front();
		tokens->pop_front();
	}
	else
		exit(-1);
	
	if (!tokens->empty())
	{
		thirdToken = tokens->front();
		tokens->pop_front();
	}
	else
		exit(-1);
	
	if (thirdToken.kind == Scanner::LPARAN)
	{	
		tokens->push_front(thirdToken);
		tokens->push_front(secondToken);
		t = parse_function_declaration();
	}
	else if (currentToken.lexeme == "const")
	{
		tokens->push_front(thirdToken);
		tokens->push_front(secondToken);
		t = parse_const_declaration();
	}
	else if (thirdToken.kind == Scanner::SEMI || thirdToken.kind == Scanner::LBRACKET)
	{
		tokens->push_front(thirdToken);
		tokens->push_front(secondToken);
	 	t = parse_var_declaration();
	}
	else
	{
		tokens->push_front(thirdToken);
		tokens->push_front(secondToken);
		syntaxError("a variable declaration of a function declaration", currentToken.lexeme);
		readNextToken();
	}
	return t;
}

Parser::TreeNode * Parser::parse_var_declaration()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = Var_declaration_kind;
	t->child[0] = parse_type();

	if (currentToken.kind == Scanner::ID)
	{
		TreeNode *p = new TreeNode();
		p->nodeKind = Identifier_kind;
		p->token = currentToken;
		t->child[1] = p;
		match(Scanner::ID, "identifier"); 
	}
	
	if (peek(Scanner::SEMI))
		return t;
	else if (peek(Scanner::LBRACKET))
	{
		t->nodeKind = Var_array_declaration_kind;
		if (currentToken.kind == Scanner::INT)
		{
			TreeNode *p = new TreeNode();
			p->nodeKind = Const_kind;
			p->token = currentToken;
			t->child[2] = p;
			match(Scanner::INT, "integer");
		}
		match(Scanner::RBRACKET, "]");
		match(Scanner::SEMI, ";");
		return t;
	}
	else
		syntaxError("[", currentToken.lexeme);
	return t;
}

Parser::TreeNode * Parser::parse_type()
{
	TreeNode *t = new TreeNode();
	if (currentToken.kind != Scanner::RW_INT && currentToken.kind && Scanner::RW_FLOAT
		&& currentToken.kind != Scanner::RW_VOID && currentToken.kind && Scanner::RW_STRING
		&& currentToken.kind != Scanner::RW_CHAR)
		syntaxError("legal type", currentToken.lexeme);
	t->nodeKind = Type_kind;
	t->token = currentToken;
	readNextToken();
	return t;
}

Parser::TreeNode * Parser::parse_function_declaration()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = Function_declaration_kind;
	t->child[0] = parse_type();
	if (currentToken.kind == Scanner::ID)
	{
		TreeNode *p = new TreeNode();
		p->nodeKind = Identifier_kind;
		p->token = currentToken;
		t->child[1] = p;
		match(Scanner::ID, "identifier");
	}
	match(Scanner::LPARAN, "(");
	t->child[2] = parse_params();
	match(Scanner::RPARAN, ")");
	t->child[3] = parse_compound_statement();
	return t;
}

Parser::TreeNode * Parser::parse_const_declaration()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = Const_declaration_kind;
	match(Scanner::RW_CONST, "const");
	t->child[0] = parse_type();
	if (currentToken.kind == Scanner::ID)
	{
		TreeNode *p = new TreeNode();
		p->nodeKind = Identifier_kind;
		p->token = currentToken;
		t->child[1] = p;
		match(Scanner::ID, "identifier");
	}
	if (peek(Scanner::SEMI))
		return t;
	else
	{
		t->nodeKind = Const_array_declaration_kind;
		match(Scanner::LBRACKET, "[");
		if (currentToken.kind == Scanner::INT)
		{
			TreeNode *p = new TreeNode();
			p->nodeKind = Const_kind;
			p->token = currentToken;
			t->child[2] = p;
			match(Scanner::INT, "identifier");
		}
		match(Scanner::RBRACKET, "]");
		match(Scanner::SEMI, ",");
	}
	return t;
}

Parser::TreeNode * Parser::parse_params()
{
	TreeNode *t = nullptr;
	if (currentToken.kind == Scanner::RW_VOID)
	{
		t = new TreeNode();
		t->nodeKind = Type_kind;
		t->token = currentToken;
		match(Scanner::RW_VOID, "void");
		return t;
	}
	else if (currentToken.kind == Scanner::RPARAN)
	{
		t = new TreeNode();
		t->nodeKind = Type_kind;
		t->token.lexeme = "void";
		return t;
	}
	else
	{
		t = parse_param_list();
		return t;
	}
}

Parser::TreeNode * Parser::parse_param_list()
{
	TreeNode *t = nullptr;
	t = parse_param();
	TreeNode *p = t;
	while (peek(Scanner::COMMA))
	{
		TreeNode *q = parse_param();
		if (q != nullptr)
		{
			if (t == nullptr)
				t = p = q;
			else
			{
				p->next = q;
				p = q;
			}
		}
	}
	return t;
}

Parser::TreeNode * Parser::parse_param()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = Param_kind;
	t->child[0] = parse_type();
	if (currentToken.kind == Scanner::ID)
	{
		TreeNode *p = new TreeNode();
		p->nodeKind = Identifier_kind;
		p->token = currentToken;
		t->child[1] = p;
		match(Scanner::ID, "identifier");
	}
	if (peek(Scanner::LBRACKET))
	{
		match(Scanner::RBRACKET, "]");
		return t;
	}
	return t;
}

Parser::TreeNode * Parser::parse_compound_statement()
{
	TreeNode *t = nullptr;
	match(Scanner::LBRACE, "{");
	t = parse_local_declarations();
	TreeNode *p = t;
	if (t != nullptr)
	{	
		while (p->next != nullptr)
			p = p->next;
		p->next = parse_statement_list();
	}
	else
	{
		t = parse_statement_list();
	}
	match(Scanner::RBRACE, "}");
	return t;
}

Parser::TreeNode * Parser::parse_local_declarations()
{
	TreeNode *t = nullptr;
	while (currentToken.kind == Scanner::RW_INT || currentToken.kind == Scanner::RW_FLOAT
		   || currentToken.kind == Scanner::RW_VOID || currentToken.kind == Scanner::RW_STRING
		   || currentToken.kind == Scanner::RW_CHAR)
		t = parse_var_declaration();
	return t;
}

Parser::TreeNode * Parser::parse_statement_list()
{
	TreeNode *t = nullptr;
	TreeNode *p = t;

	if (currentToken.kind != Scanner::RBRACE)
	{
		t = parse_statement();
		p = t;
	}
	while (currentToken.kind != Scanner::RBRACE)
	{
		TreeNode *q = parse_statement();
		if (q != nullptr)
		{
			if (t == nullptr)
			{
				t = p = q;
			}
			else
			{
				p->next = q;
				p = q;
			}
		}
	}
	return t;
}

Parser::TreeNode * Parser::parse_statement()
{
	TreeNode *t = nullptr;
	if (currentToken.kind == Scanner::RW_RETURN)
		t = parse_return_statement();
	else if (currentToken.kind == Scanner::RW_WHILE)
		t = parse_iteration_statement();
	else if (currentToken.kind == Scanner::RW_IF)
		t = parse_selection_statement();
	else if (currentToken.kind == Scanner::LBRACE)
		t = parse_compound_statement();
	else
		t = parse_expression_statement();
	return t;
}

Parser::TreeNode * Parser::parse_expression_statement()
{
	TreeNode *t = nullptr;
	if (peek(Scanner::SEMI))
		return t;
	else
	{
		t = parse_expression();
		match(Scanner::SEMI, ";");
		return t;
	}
}

Parser::TreeNode * Parser::parse_selection_statement()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = If_kind;
	t->token = currentToken;
	match(Scanner::RW_IF, "if");
	match(Scanner::LPARAN, "(");
	t->child[0] = parse_expression();
	match(Scanner::RPARAN, ")");
	t->child[1] = parse_statement();
	if (peek(Scanner::RW_ELSE))
		t->child[2] = parse_statement();
	return t;
}

Parser::TreeNode * Parser::parse_iteration_statement()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = While_kind;
	t->token = currentToken;
	match(Scanner::RW_WHILE, "while");
	match(Scanner::LPARAN, "(");
	t->child[0] = parse_expression();
	match(Scanner::RPARAN, ")");
	t->child[1] = parse_statement();
	return t;
}

Parser::TreeNode * Parser::parse_return_statement()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = Return_kind;
	t->token = currentToken;
	match(Scanner::RW_RETURN, "return");
	if (peek(Scanner::SEMI))
		return t;
	else
	{
		t->child[0] = parse_expression();
		match(Scanner::SEMI, ";");
	}
	return t;
}

Parser::TreeNode * Parser::parse_var()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = Object_kind;
	t->token = currentToken;
	match(Scanner::ID, "identifier");
	if (peek(Scanner::LBRACKET))
	{
		t->child[0] = parse_expression();
		match(Scanner::RBRACKET, "]");
	}
	return t;
}

Parser::TreeNode * Parser::parse_expression()
{
	TreeNode *t = nullptr;
	t = parse_additive_expression();
	if (currentToken.lexeme == "=" || currentToken.lexeme == "<" || currentToken.lexeme == ">" ||
		currentToken.lexeme == "==" || currentToken.lexeme == ">=" || currentToken.lexeme == "<=" ||
		currentToken.lexeme == "!=")
	{
		TreeNode *p = new TreeNode();
		if (currentToken.lexeme == "=")
			p->nodeKind = Assign_kind;
		else
			p->nodeKind = Operation_kind;
		p->token = currentToken;
		p->child[0] = t;
		t = p;
		readNextToken();
		t->child[1] = parse_additive_expression();
	}
	return t;
}

Parser::TreeNode * Parser::parse_additive_expression()
{
	TreeNode *t = nullptr;
	t = parse_term();
	while (currentToken.lexeme == "+" || currentToken.lexeme == "-")
	{
		TreeNode *p = new TreeNode();
		p->nodeKind = Operation_kind;
		p->token = currentToken;
		p->child[0] = t;
		t = p;
		readNextToken();
		t->child[1] = parse_term();
	}
	return t;
}

Parser::TreeNode * Parser::parse_term()
{
	TreeNode *t = nullptr;
	t = parse_factor();
	while (currentToken.lexeme == "*" || currentToken.lexeme == "/")
	{
		TreeNode *p = new TreeNode();
		p->nodeKind = Operation_kind;
		p->token = currentToken;
		p->child[0] = t;
		t = p;
		readNextToken();
		t->child[1] = parse_factor();
	}
	return t;
}

Parser::TreeNode * Parser::parse_factor()
{
	TreeNode *t = nullptr;
	if (currentToken.kind == Scanner::INT || currentToken.kind == Scanner::CHAR || 
		currentToken.kind == Scanner::FLOAT || currentToken.kind == Scanner::STRING)
	{
		t = new TreeNode();
		t->nodeKind = Const_kind;
		t->token = currentToken;
		match(currentToken.kind, currentToken.lexeme);
		return t;
	}

	if (peek(Scanner::LPARAN))
	{
		t = parse_expression();
		match(Scanner::RPARAN, "(");
	}
	else
	{
		if (!tokens->empty())
			secondToken = tokens->front();
		else
			exit(-1);
		if (currentToken.kind == Scanner::ID && secondToken.kind == Scanner::LPARAN)
			t = parse_call();
		else
			t = parse_var();
	}
	return t;
}

Parser::TreeNode * Parser::parse_call()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = Call_kind;
	if (currentToken.kind == Scanner::ID)
	{
		TreeNode *p = new TreeNode();
		p->nodeKind = Identifier_kind;
		p->token = currentToken;
		t->child[0] = p;
		match(Scanner::ID, "identifier");
	}
	match(Scanner::LPARAN, "(");
	t->child[1] = parse_args();
	match(Scanner::RPARAN, ")");
	return t;
}

Parser::TreeNode * Parser::parse_args()
{
	TreeNode *t = nullptr;
	if (currentToken.kind != Scanner::RPARAN)
		t = parse_expression();
	TreeNode *p = t;
	while (currentToken.kind == Scanner::COMMA)
	{
		match(Scanner::COMMA, ",");
		TreeNode *q = parse_expression();
		if (q != nullptr)
		{
			if (t == nullptr)
			{
				t = p = q;
			}
			else
			{
				p->next = p;
				p = q;
			}
		}
	}
	return t;
}
