#include <iostream>
#include "Error.h"

using namespace std;

int errorNum;

bool hasError()
{
    return errorNum;
}

void syntaxError(string currentParserFilename, string expected, Scanner::Token token)
{
    errorNum++;
    cerr << "Error in class " << currentParserFilename << " in line " << token.row
        << ": expect a " << expected << ", but got a " << token.lexeme << "\n";
}

// 类名和函数名不一致
void error1(string currentParserFilename)
{
    errorNum++;
    cerr << "Error in file " << currentParserFilename << ".jack: " << "classname should be same as filename" << endl;
}

// 变量重定义
void error2(string currentClass, int row, string type, string name)
{
    errorNum++;
    cerr << "Error in class " << currentClass << " in line " << row
        << ": redeclaration of '" << type << " " << name << "'" << endl;
}

// 函数重定义
void error3(string currentClass, int row, string type, string name)
{
    errorNum++;
    cerr << "Error in class " << currentClass << " in line " << row
        << ": redeclaration of '" << type << " " << name << "()" << endl;
}

// 类型未定义
void error4(string currentClassName, int row, string type)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": '" << type << "' not declaraed" << endl;
}

// 变量未定义
void error5(string currentClassName, int row, string varName)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": '" << varName << "' does not declared in this scope" << endl;
}

void error6(string currentClassName, int row, string type)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": " << type << " does not an Array type" << endl;
}

void error7(string currentClassName, string callerName, int row, string functionName)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": class " << callerName << " haven't a member function '" << functionName << "()'" << endl;
}

void error8(string currentClassName, int row, string functionName)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": subroutine " << functionName << " called as a method from within a function" << endl;
}

void error9(string currentClassName, string callerName, int row, string functionName)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": '" << functionName << "' is not a function in class " << callerName << endl;
}

// 函数类型错误
void error10(string currentClassName, string callerName, int row, string functionName)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": '" << functionName << "' is not a method in class " << callerName << endl;
}

// 返回值错误
void error11(string currentClassName, string type, int row)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": return-statement with no value, in function returning '" << type << "'" << endl;
}

// 返回值错误
void error12(string currentClassName, int row)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": return-statement with a value, in function returning void" << endl;
}

// 返回值错误
void error13(string currentClassName, int row)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": The return type of a constructor must be of the class type" << endl;
}

// 参数太少
void error14(string currentClassName, string functionName, int row)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": too few arguments to function " << functionName << "()" << endl;
}

// 参数太多
void error15(string currentClassName, string functionName, int row)
{
    errorNum++;
    cerr << "Error in class " << currentClassName << " in line " << row
        << ": too many arguments to function " << functionName << endl;
}

void error16()
{
    errorNum++;
    cerr << "Error: Main class not exsist" << endl;
}

void error17()
{
    errorNum++;
    cerr << "Error in class Main: main function does not exsit!" << endl;
}

void error18()
{
    errorNum++;
    cerr << "Error in class Main: the kind of subroutine main must be a function" << endl;
}

void error19()
{
    errorNum++;
    cerr << "Error in class Main: the type of subroutine main must be a void" << endl;
}

void error20()
{
    errorNum++;
    cerr << "Error in class Main: the argument size of subroutine main must be null" << endl;
}
