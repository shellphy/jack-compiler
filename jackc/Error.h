#ifndef _ERROR_H
#define _ERROR_H

#include "Scanner.h"

extern int errorNum;

bool hasError();
void syntaxError(string currentParserFilename, string expected, Scanner::Token token);

// 类名和函数名不一致
void error1(string currentParserFilename);

// 变量重定义
void error2(string currentClass, int row, string type, string name);

// 函数重定义
void error3(string currentClass, int row, string type, string name);

// 类型未定义
void error4(string currentClassName, int row, string type);

// 变量未定义
void error5(string currentClassName, int row, string varName);

// 类型不匹配
void error6(string currentClassName, int row, string type);

// 函数未定义
void error7(string currentClassName, string callerName, int row, string functionName);

// 函数类型不一样
void error8(string currentClassName, int row, string functionName);

// 函数类型错误
void error9(string currentClassName, string callerName, int row, string functionName);

// 函数类型错误
void error10(string currentClassName, string callerName, int row, string functionName);

// 返回值错误
void error11(string currentClassName, string type, int row);

// 返回值错误
void error12(string currentClassName, int row);

// 返回值错误
void error13(string currentClassName, int row);

// 参数太少
void error14(string currentClassName, string functionName, int row);

// 参数太多
void error15(string currentClassName, string functionName, int row);

void error16();

void error17();

void error18();

void error19();

void error20();

#endif
