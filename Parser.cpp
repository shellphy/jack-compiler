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
		case Parser::compound_statement_kind:
			cout << "compound_statement" << endl;
			break;
		case Parser::Bool_kind:
			cout << tree->token.lexeme << endl;
			break;
		case Parser::Var_declaration_kind:				
			cout << "var declaration" << endl;
			break;
		case Parser::Var_array_declaration_kind:		
			cout << "array declaratoin" << endl;			
			break;
		case Parser::Function_declaration_kind:			
			cout << "function declaration" << endl;			
			break;
		case Parser::Param_kind:						
			cout << "param" << endl;			
			break;
		case Parser::Param_Array_kind:
			cout << "array declaration" << endl;
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
		case Parser::Break_kind:
			cout << "break" << endl;
			break;
		case Parser::Continue_kind:
			cout << "continue" << endl;
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
	error = false;
	errorNumbers = 1;
}

void Parser::parse()
{
	scanner.scanToken();
	tokens = scanner.getTokens();
	syntaxTree = parse_program();
	cout << "**************************syntaxTree*****************************" << endl;
}

void Parser::syntaxError(string got)
{
	cerr << "\n>>>error (" << Parser::errorNumbers << ") ("
		<< currentToken.currentRow << ", " << currentToken.currentColumn << ") " << endl;
	cerr << "\tunexpected token " << got << endl;
	Parser::errorNumbers++;
	error = true;
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

void Parser::match(string expectedString)
{
	if (currentToken.lexeme == expectedString)
	{
		readNextToken();
	}
	else
	{
		syntaxError(expectedString);
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

/*
文法: declaration_list -> declaration_list declaration
                        | declaration

语法树:  declaration -> declaration -> declaration -> ... -> declaration
*/
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


/*
文法: declaration -> var_declaration
                   | function_declaration
这个文法的解析步骤:
  通过观察两种不同的声明格式,可知:
    变量声明的形式是: int a; 或者是int a[];
    函数声明的形式是: int func(){}
  所以需要向前看2个Token才知道到底该调用哪一个产生式.
    如果向前看的第2个Token是左括号,那么就调用function_declaration()
    否则就调用var_declaration()
*/
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
		return t;
	}
	else
	{
		tokens->push_front(thirdToken);
		tokens->push_front(secondToken);
		t = parse_var_declaration();
		return t;
	}
}

/*
文法: var_declaration -> type ID ;
                       | type ID [ INT ] ;

语法树:
        int a;
	Var_declaration_kind
         /       \
 	   int        a

	   int array[3];
	Var_array_declaration_kind
	    /    |    \
	  int  array   3
*/

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
		readNextToken();
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
			readNextToken();
		}
		else
		{
			syntaxError(currentToken.lexeme);
		}
		match("]");
		match(";");
		return t;
	}
	else
	{
		syntaxError(currentToken.lexeme);
		return t;
	}
}


/*
文法: type -> int
            | float
            | void
            | string
            | char
*/
Parser::TreeNode * Parser::parse_type()
{
	TreeNode *t = new TreeNode();
	if (currentToken.kind != Scanner::RW_INT && currentToken.kind != Scanner::RW_FLOAT
		&& currentToken.kind != Scanner::RW_VOID && currentToken.kind != Scanner::RW_STRING
		&& currentToken.kind != Scanner::RW_CHAR && currentToken.kind != Scanner::RW_BOOL)
		syntaxError(currentToken.lexeme);
	t->nodeKind = Type_kind;
	t->token = currentToken;
	readNextToken();
	return t;
}

/*
文法: function_declaration -> type ID ( params ) compound_statement 

语法树:
    int func(int a, int b)
	{
		......
	}
	     Function_declaration_kind
		    /    |     |      \
		  int  func  params  compound_statement
*/

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
		readNextToken();
	}
	match("(");
	t->child[2] = parse_params();
	match(")");
	t->child[3] = parse_compound_statement();
	return t;
}

/*
文法: params -> param_list 
              | void
              | empty

文法的解析: 如果当前的Token的类型是不是void, 如果是, 那么直接生成一个void类型的单节点
            如果当前的Token是不是一个右括号, 如果是, 还是生成一个void类型的单节点
			否则就说明至少有一个参数, 于是就解析param_list
*/
Parser::TreeNode * Parser::parse_params()
{
	TreeNode *t = nullptr;
	if (currentToken.kind == Scanner::RW_VOID)
	{
		t = new TreeNode();
		t->nodeKind = Type_kind;
		t->token = currentToken;
		readNextToken();
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

/*
文法: param_list -> param_list , param
                  | param

文法的解析: 这是一个左递归的文法,递归展开之后的形式为: param, param, param, ... , param
            也就是说至少有一个param, 所以先解析第一个param, 解析完了之后, 如果当前Token是一个逗号, 
			那么就证明后面还有param, 不断循环
语法树;
    param -> param -> param -> ... -> param
*/
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

/*
文法: param -> type ID
             | type ID [ ]			 
*/

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
		readNextToken();
	}
	if (peek(Scanner::LBRACKET))
	{
		TreeNode *p = new TreeNode();
		p->nodeKind = Param_Array_kind;
		t->child[2] = p;
		match("]");
		return t;
	}
	return t;
}


/*
文法: compound_statement -> { local_declarations statement_list }   

*/
Parser::TreeNode * Parser::parse_compound_statement()
{
	TreeNode *t = nullptr;
	match("{");
	t = new TreeNode();
	t->nodeKind = compound_statement_kind;
	t->child[0] = parse_local_declarations();
	TreeNode *p = t->child[0];
	if (t->child[0] != nullptr)
	{	
		while (p->next != nullptr)
			p = p->next;
		p->next = parse_statement_list();
	}
	else
	{
		t->child[0] = parse_statement_list();
	}
	match("}");

	return t;
}

/*
文法: local_declarations -> local_declarations var_declaration
                          | empty
						  
文法的解析: 先考虑第一个产生式, 这是一个左递归的, 类似于前面的param_list, 展开之后至少有一个var_declaration
            但是第二个产生式中可以是空, 所以最后的推导是0个或多个var_declaration
			而对于var_declaration的判断, 可以向前看一个Token来推断是否该继续展开
*/
Parser::TreeNode * Parser::parse_local_declarations()
{
	TreeNode *t = nullptr;
	TreeNode *p = t;
	while (currentToken.kind == Scanner::RW_INT || currentToken.kind == Scanner::RW_FLOAT
		|| currentToken.kind == Scanner::RW_VOID || currentToken.kind == Scanner::RW_STRING
		|| currentToken.kind == Scanner::RW_CHAR || currentToken.kind == Scanner::RW_BOOL)
	{
		TreeNode *q = parse_var_declaration();
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
	return t;
}

/*
文法: statement_list -> statement_list statement
                      | empty

文法的解析: 这个文法的解析比较特殊. 文法递归展开之后是0个或多个statement, 而一个statement有很多种类型
            用向前看的方式是不行的.
			考虑到这个推导: compound_statement -> { local_declarations statement_list }   
			可以判断: 如果一个statement结束了, 那么接下来的Token必定是接着一个右花括号
			所以判断当前的Token是不是右花括号来决定是否继续展开statement
*/

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

/*
文法: statement -> expression_statement
                 | compound_statement
                 | selection_statement
                 | iteration_statement
                 | return_statement
                 | assign_statement
				 | break_statement
				 | continue_statement
*/

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
	else if (currentToken.kind == Scanner::RW_BREAK)
	{
		t = new TreeNode();
		t->nodeKind = Break_kind;
		t->token = currentToken;
		readNextToken();
	}
	else if (currentToken.kind == Scanner::RW_CONTINUE)
	{
		t = new TreeNode();
		t->nodeKind = Continue_kind;
		t->token = currentToken;
		readNextToken();
	}
	else
		t = parse_expression_statement();
	return t;
}


/*
文法: expression_statement -> expression ;
                      | empty ;

解析: 类似于statement_list, 是看当前的Token是不是分号, 如果不是, 那么就说明是一个expression, 如果是, 那么就是empty
					  */
Parser::TreeNode * Parser::parse_expression_statement()
{
	TreeNode *t = nullptr;
	if (peek(Scanner::SEMI))
		return t;
	else
	{
		t = parse_expression();
		match(";");
		return t;
	}
}

/*
文法: selection_statement -> if ( expression ) statement
                           | if ( expression ) statement else statement

说明: 因为解析的过程是最左推导, 所以只存在一种语法树, 不会产生二义性
*/
Parser::TreeNode * Parser::parse_selection_statement()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = If_kind;
	t->token = currentToken;
	match("if");
	match("(");
	t->child[0] = parse_expression();
	match(")");
	t->child[1] = parse_statement();
	if (peek(Scanner::RW_ELSE))
		t->child[2] = parse_statement();
	return t;
}

/*
文法: iteration_statement -> while ( expression ) statement

*/
Parser::TreeNode * Parser::parse_iteration_statement()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = While_kind;
	t->token = currentToken;
	match("while");
	match("(");
	t->child[0] = parse_expression();
	match(")");
	t->child[1] = parse_statement();
	return t;
}

/*
文法: return_statement -> return ;
                        | return expression ;
*/
Parser::TreeNode * Parser::parse_return_statement()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = Return_kind;
	t->token = currentToken;
	match("return");
	if (peek(Scanner::SEMI))
		return t;
	else
	{
		t->child[0] = parse_expression();
		match(";");
	}
	return t;
}

/*
文法: var -> ID
           | ID [ expression ]
*/
Parser::TreeNode * Parser::parse_var()
{
	TreeNode *t = new TreeNode();
	t->nodeKind = Identifier_kind;
	t->token = currentToken;
	readNextToken();
	if (peek(Scanner::LBRACKET))
	{
		t->child[0] = parse_expression();
		match("]");
	}
	return t;
}

/*
文法: expression -> expression && boolexpression
                 | expression || boolexpression
                 | boolexpression
*/
Parser::TreeNode * Parser::parse_expression()
{
	TreeNode *t = nullptr;
	t = parse_boolexpression();
	while (currentToken.lexeme == "&&" || currentToken.lexeme == "||")
	{
		TreeNode *p = new TreeNode();
		p->nodeKind = Bool_kind;
		p->token = currentToken;
		p->child[0] = t;
		t = p;
		readNextToken();
		t->child[1] = parse_boolexpression();
	}
	return t;
}

/*
文法: boolexpression -> additive_expression relational_operator additive_expression
                    | additive_expression
*/
Parser::TreeNode * Parser::parse_boolexpression()
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

/*
文法: additive_expression -> additive_expression + term
                           | additive_expression – term
                           | term
*/
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

/*
文法: term -> term * factor
            | term / factor
            | factor
*/
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

/*
文法: factor -> ( expression )
              | var
              | call
              | INT
              | FLOAT
              | CHAR
              | STRING
*/
Parser::TreeNode * Parser::parse_factor()
{
	TreeNode *t = nullptr;
	if (currentToken.kind == Scanner::INT || currentToken.kind == Scanner::CHAR 
	 || currentToken.kind == Scanner::FLOAT || currentToken.kind == Scanner::STRING
	 || currentToken.kind == Scanner::BOOL)
	{
		t = new TreeNode();
		t->nodeKind = Const_kind;
		t->token = currentToken;
		readNextToken();
		return t;
	}

	if (peek(Scanner::LPARAN))
	{
		t = parse_expression();
		match(")");
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

/*
文法: call -> ID ( args )
*/
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
		readNextToken();
	}
	match("(");
	t->child[1] = parse_args();
	match(")");
	return t;
}

/*
文法: args -> arg_list 
            | empty
*/
Parser::TreeNode * Parser::parse_args()
{
	TreeNode *t = nullptr;
	if (currentToken.kind != Scanner::RPARAN)
		t = parse_expression();
	TreeNode *p = t;
	while (currentToken.kind == Scanner::COMMA)
	{
		match(",");
		TreeNode *q = parse_expression();
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

bool Parser::hasError()
{
	return error;
}
