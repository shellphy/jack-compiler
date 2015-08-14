#ifndef _Scanner_H_
#define _Scanner_H_

#include <string>
#include <fstream>
#include <deque>
#include <set>

using namespace std;

class Scanner
{
private:
    enum State	// 转移图中的状态
    {
        START_STATE,		// 开始状态
        ID_STATE,			// 标识符状态
        INT_STATE,			// 整型数状态
        CHAR_STATE,			// 字符状态		
        CHAR_STATE_A,
        CHAR_STATE_B,
        CHAR_STATE_C,
        FLOAT_STATE,		// 浮点数状态
        D_FLOAT_STATE,		// 接近带小数点的浮点数状态
        E_FLOAT_STATE,		// 接近科学技术法的浮点数状态
        STRING_STATE,		// 字符串状态
        S_STRING_STATE,		// 含有转移字符的字符串
        SYMBOL_STATE, 
        INCOMMENT_STATE,	// 注释状态
        P_INCOMMENT_STATE,	// 快要结束注释状态
        DONE_STATE,			// 结束状态
        ERROR_STATE			// 错误状态
    };

public:
    set<string> keyWords;
    set<string> symbols;
    enum TokenType
    {      
        KEY_WORD,
        ID,				// 标识符
        INT,			// 整型数字
        BOOL,			// 布尔类型
        CHAR,			// 字符
        STRING,			// 字符串
        SYMBOL,         // 合法的符号
        NONE,		    // 无类型
        ERROR,		    // 错误
        ENDOFFILE	    // 文件结束
    };
    struct Token
    {
        TokenType kind;				// Token的类型
        string lexeme;				// Token的值
        unsigned row;	   	        // 当前行
    };
    void initKeyWords();
    void initSymbols();
private:
    string lineBuffer;					// 缓冲行, 保存源程序中的一行数据
    unsigned bufferPos;					// 缓冲行的指针
    unsigned row;						// 保存当前缓冲行在源程序中的行号
    ifstream fin;						// 源程序文件的输入流对象
    char nextChar();					// 返回缓冲区中的下一个字符
    void rollBack();					// 回滚缓冲区
    TokenType searchReserved(string &s);	// 查找关键字
public:
    Scanner();
    void openFile(string filename);
    void closeFile();
    Token nextToken();					// 返回下一个Token
    void resetRow();
};

#endif
