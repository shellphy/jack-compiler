#ifndef _VIRTUAL_MACHION_H
#define _VIRTUAL_MACHION_H

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

extern vector<string> filenames;

void executeArithmetic(string command);                         // 执行算术指令
void executePush(string segment, int index);                    // 执行push指令
void executePop(string segment, int index);                     // 执行pop指令
void executeLabel(string label);                                // 执行label指令
void executeGoto(string label);                                 // 执行goto指令
void executeIf(string label);                                   // 执行if-goto指令
void executeCall(string functionName, int numArgs);             // 执行call指令
void executeReturn();                                           // 执行return指令
void executeFunction(string functionName, int numLocals);       // 执行function指令
void executeEnd();                                              // 程序结束

void init();                                                    // cpu通电之后初始化ip
void instructionFetch();                                        // cpu取指令
void execute();                                                 // cpu执行指令

void setKeyboardValue(short val);
void loadProgram();                                             // 载入程序到指令存储器中
void run();                                                     // CPU通电开始运行

#endif
