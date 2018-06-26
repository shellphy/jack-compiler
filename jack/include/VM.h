#ifndef _VM_H
#define _VM_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

extern vector<string> filenames;

void executeArithmetic(string const& command);                   // 执行算术指令
void executePush(string const& segment, int index);              // 执行push指令
void executePop(string const& segment, int index);               // 执行pop指令
void executeLabel(string const& label);                          // 执行label指令
void executeGoto(string const& label);                           // 执行goto指令
void executeIf(string const& label);                             // 执行if-goto指令
void executeCall(string const& functionName, int numArgs);       // 执行call指令
void executeReturn();                                     // 执行return指令
void executeFunction(string const& functionName, int numLocals); // 执行function指令
void executeEnd();                                        // 程序结束

void init();             // cpu通电之后初始化ip
void instructionFetch(); // cpu取指令
void execute();          // cpu执行指令

void setKeyboardValue(short val);
void loadProgram(); // 载入程序到指令存储器中
void run();         // CPU通电开始运行

#endif
