#include "Scanner.h"
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <iterator>

using namespace std;

Scanner::Scanner(string sourceFile)
{
	row = column = 0;
	isEof = false;
	bufferPos = 0;
	
	reservedWords[0] = { RESERVEDWORD, "if", 0, 0 };
	reservedWords[1] = { RESERVEDWORD, "else", 0, 0 };
	reservedWords[2] = { RESERVEDWORD, "int", 0, 0 };
	reservedWords[3] = { RESERVEDWORD, "return", 0, 0 };
	reservedWords[4] = { RESERVEDWORD, "void", 0, 0 };
	reservedWords[5] = { RESERVEDWORD, "while", 0, 0 };

	fin.open(sourceFile.c_str());
	if (fin.fail())
	{
		std::cerr << "文件打开失败， 文件"
			<< sourceFile << "不存在" << std::endl;
		exit(-1);
	}
}

Scanner* Scanner::getInstance(string sourceFile)
{
	Scanner *scanner = new Scanner(sourceFile);
	return scanner;
}

Scanner::TokenType Scanner::searchReserved(string &s)
{
	for (auto it = begin(reservedWords); it != end(reservedWords); ++it)
		if (it->lexeme == s)
			return it->kind;
	return ID;
}

char Scanner::nextChar()
{
	if (bufferPos >= lineBuffer.size())
	{
		row++;
		getline(fin, lineBuffer);
		lineBuffer += '\n';
		if (!fin.fail())
		{
			bufferPos = 0;
			return lineBuffer[bufferPos++];
		}
		else
			return EOF;
	}
	else
		return lineBuffer[bufferPos++];
}

void Scanner::rollBack()
{
	assert(bufferPos > 0);
	bufferPos--;
}

void Scanner::scanToken()
{
	Token token = nextToken();
	while (token.kind != ENDOFFILE)
	{
		tokens.push_back(token);
		token = nextToken();
	}
}

deque<Scanner::Token> Scanner::getTokens()
{	
	return tokens;
}

void Scanner::output()
{
	scanToken();
	while (!tokens.empty())
	{
		auto token = tokens.front();
		tokens.pop_front();
		cout << " " << token.lexeme << "\t(" << token.currentRow << ", " << token.currentColumn << ")" << endl;
	}
}

Scanner::Token Scanner::nextToken()
{
	Token token;
	unsigned tokenStringIndex = 0;
	State state = START_STATE;
	while (state != DONE_STATE)
	{
		char ch = nextChar();
		if (ch == EOF)
		{
			isEof = true;
			token.kind = ENDOFFILE;
			break;
		}
		switch (state)
		{
		case START_STATE:										// 开始状态
			column = bufferPos;
			if (ch == ' ' || ch == '\t' || ch == '\n')
				;
			else if (isalpha(ch))
			{
				state = ID_STATE;		// 进入标识符状态
				token.kind = ID;
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (isdigit(ch))
			{
				state = NUM_STATE;		// 进入整数状态
				token.kind = NUM;	
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '+')
			{
				state = PLUS_STATE;		// 进入加号状态
				token.kind = PLUS;	
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '-')
			{
				state = MINUS_STATE;	// 进入减号状态
				token.kind = MINUS;		
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '*')
			{
				state = MULT_STATE;		// 进入乘号状态
				token.kind = MULT;		
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '/')
			{
				state = DIVIDE_STATE;	// 进入除号状态
				token.kind = DIVIDE;	
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '(')
			{
				state = LPARAN_STATE;	// 进入左括号状态
				token.kind = LPARAN;	
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == ')')
			{
				state = RPARAN_STATE;	// 进入右括号状态
				token.kind = RPARAN;	
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '[')
			{
				state = LBRACE_STATE;	// 进入左中括号状态
				token.kind = LBRACE;	
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == ']')
			{
				state = RBRACE_STATE;	// 进入右中括号状态
				token.kind = RBRACE;	
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '{')
			{
				state = LBRACE_STATE;	// 进入左花括号状态
				token.kind = LBRACE;	
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '}')
			{
				state = RBRACE_STATE;	// 进入右花括号状态
				token.kind = RBRACE;
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == ';')
			{
				state = SEMI_STATE;		// 进入分号状态
				token.kind = SEMI;		
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == ',')
			{
				state = COMMA_STATE;	// 进入逗号状态
				token.kind = COMMA;		
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '<')
			{
				state = LT_STATE;		// 进入小于号状态
				token.kind = LT;		
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '>')
			{
				state = GT_STATE;		// 进入大于号状态
				token.kind = GT;		
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '=')
			{
				state = ASSIGN_STATE;	// 进入赋值号状态
				token.kind = ASSIGN;	
				token.lexeme += ch;
				token.currentRow = row;
				token.currentColumn = column;
			}
			else if (ch == '!')
			{
				state = IN_NEQ_STATE;	// 进入接近不等于状态
				token.lexeme += ch;		
				token.currentRow = row;
				token.currentColumn = column;
			}
			break;
		case NUM_STATE:											// 整数状态
			if (isalpha(ch))
			{
				state = ERROR_STATE;
				token.kind = ERROR;
				token.lexeme += ch;
				break;
			}
			else if (isdigit(ch))
			{
				token.lexeme += ch;
				break;
			}
			else
			{
				rollBack();
				state = DONE_STATE;
				break;
			}
		case ID_STATE:											// 标识符状态
			if (!isalnum(ch))
			{
				rollBack();
				state = DONE_STATE;
				break;
			}
			else
			{
				token.lexeme += ch;
				break;
			}
		case LPARAN_STATE:										// 左括号状态
			rollBack();
			state = DONE_STATE;
			break;
		case RPARAN_STATE:										// 右括号状态
			rollBack();
			state = DONE_STATE;
			break;
		case LBRACE_STATE:										// 左花括号状态
			rollBack();
			state = DONE_STATE;
			break;
		case RBRACE_STATE:										// 右花括号状态
			rollBack();
			state = DONE_STATE;
			break;
		case LBRACKET_STATE:									// 左中括号状态
			rollBack();
			state = DONE_STATE;
			break;
		case RBRACKET_STATE:									// 右中括号状态
			rollBack();
			state = DONE_STATE;
			break;
		case SEMI_STATE:										// 分号状态
			rollBack();
			state = DONE_STATE;
			break;
		case COMMA_STATE:										// 逗号状态
			rollBack();
			state = DONE_STATE;
			break;
		case PLUS_STATE:										// 加号状态
			rollBack();
			state = DONE_STATE;
			break;
		case MINUS_STATE:										// 减号状态
			rollBack();
			state = DONE_STATE;
			break;
		case MULT_STATE:										// 乘号状态
			rollBack();
			state = DONE_STATE;
			break;
		case DIVIDE_STATE:										// 除号状态
			if (ch != '*')
			{
				rollBack();
				state = DONE_STATE;
				break;
			}
			else
			{
				state = INCOMMENT_STATE;
				token.lexeme.pop_back();
				break;
			}
		case ASSIGN_STATE:
			if (ch == '=')
			{
				state = EQ_STATE;
				token.kind = EQ;
				token.lexeme += ch;
				break;
			}
			else
			{
				rollBack();
				state = DONE_STATE;
				break;
			}
		case LT_STATE:											// 小于号状态
			if (ch == '=')
			{
				state = LE_STATE;
				token.kind = LE;
				token.lexeme += ch;
				break;
			}
			else
			{
				rollBack();
				state = DONE_STATE;
				break;
			}
		case GT_STATE:											// 大于号状态
			if (ch == '=')
			{
				state = GE_STATE;
				token.kind = GE;
				token.lexeme += ch;
				break;
			}
			else
			{
				rollBack();
				state = DONE_STATE;
				break;
			}
		case IN_NEQ_STATE:										// 接近不等于号状态
			if (ch == '=')
			{
				state = NEQ_STATE;
				token.kind = NEQ;
				token.lexeme += ch;
				break;
			}
			else
			{
				state = ERROR_STATE;
				token.kind = ERROR;
				token.lexeme += ch;
				break;
			}
		case LE_STATE:											// 小于等于号状态
			rollBack();
			token.kind = LE;
			break;
		case GE_STATE:											// 大于等于号状态
			rollBack();
			token.kind = GE;
			break;	
		case NEQ_STATE:											// 不等于号状态
			rollBack();
			token.kind = NEQ;
			break;
		case ERROR_STATE:										// 错误状态
			if (ch == ' ' || ch == '\n' || ch == '\t')
				state = DONE_STATE;
			else
				token.lexeme += ch;
			break;
		case INCOMMENT_STATE:									// 注释状态
			if (ch == '*')
				state = P_INCOMMENT_STATE;
			break;
		case P_INCOMMENT_STATE:									// 接近退出注释状态
			if (ch == '/')
				state = START_STATE;
			else
				state = INCOMMENT_STATE;
			break;
		}
		if (state == DONE_STATE && token.kind == ID)
			token.kind = searchReserved(token.lexeme);
	}
	return token;
}
