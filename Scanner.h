#ifndef _Scanner_H_
#define _Scanner_H_

#include <string>
#include <fstream>
#include <deque>

using namespace std;

class Scanner
{
private:
	enum State	// 转移图中的状态
	{
		START_STATE,		// 开始状态
		ID_STATE,			// 标识符状态
		NUM_STATE,			// 整型数状态
		PLUS_STATE,			// 加状态
		MINUS_STATE,		// 减状态
		MULT_STATE,			// 乘状态
		DIVIDE_STATE,		// 除状态	
		ASSIGN_STATE,		// 赋值状态
		EQ_STATE,			// 等于状态
		IN_NEQ_STATE,		// 接近不等于状态
		NEQ_STATE,			// 不等于状态
		LT_STATE,			// 小于状态
		LE_STATE,			// 小于等于状态
		GT_STATE,			// 大于状态
		GE_STATE,			// 大于等于状态
		LPARAN_STATE,		// 左括号状态
		RPARAN_STATE,		// 右括号状态						
		SEMI_STATE,			// 分号状态
		COMMA_STATE,		// 逗号状态
		LBRACKET_STATE,		// 左中括号状态
		RBRACKET_STATE,		// 右中括号状态
		LBRACE_STATE,		// 左花括号状态
		RBRACE_STATE,		// 右花括号状态

		INCOMMENT_STATE,	// 注释状态
		P_INCOMMENT_STATE,	// 快要结束注释状态
		DONE_STATE,			// 结束状态
		ERROR_STATE			// 错误状态
	};

public:
	enum TokenType
	{
		RESERVEDWORD,	// 保留字	
		ID,				// 标识符
		NUM,			// 无符号整型数字

		// 特殊符号
		PLUS,		// 加										+
		MINUS,		// 减										-
		MULT,		// 乘										*
		DIVIDE,		// 除										/
		ASSIGN,		// 赋值										=
		EQ,			// 等于 equal								==
		NEQ,		// 不等于 not equal                         !=
		LT,			// 小于 less than							<
		LE,			// 小于等于 less than or equal				<=
		GT,			// 大于 greater than						>
		GE,			// 大于等于  greater than or equal			>=
		LPARAN,		// 左括号									(
		RPARAN,		// 右括号									)
		SEMI,		// 分号										;
		COMMA,		// 逗号										,
		LBRACKET,	// 左中括号									[
		RBRACKET,	// 右中括号									]
		LBRACE,		// 左花括号									{
		RBRACE,		// 右花括号									}

		ERROR,		// 错误
		ENDOFFILE	// 文件结束
	};
private:
	struct Token
	{
		TokenType kind;				// Token的类型
		string lexeme;				// Token的值
		unsigned currentRow;	   	// 当前行
		unsigned currentColumn;		// 当前列
	};

	Token reservedWords[6];				// 关键字组数
	string lineBuffer;					// 缓冲行, 保存源程序中的一行数据
	unsigned bufferPos;					// 缓冲行的指针
	unsigned row;						// 标记当前缓冲行在源程序中的行
	unsigned column;					// 标记当前Token在缓冲行中的列
	bool isEof;							// 标记是否读取到文件结束
	deque<Token> tokens;				// 保存源程序中所有的Token
	ifstream fin;						// 源程序文件的输入流对象
	
	Scanner(string sourceFile);			// 构造函数
	char nextChar();					// 返回缓冲区中的下一个字符
	void rollBack();					// 回滚缓冲区
	Token nextToken();					// 返回下一个Token
	void scanToken();					// 扫描所有的Token, 保存到tokens中
	TokenType searchReserved(string &s);	// 查找关键字
public:
	static Scanner* getInstance(string sourceFile = "input.c");	// 创建Scanner的单实例
	deque<Token> getTokens();		// 返回源程序中所有的Token

	// 测试函数
	void output();					// 输出源程序中的所有Token
};

#endif
