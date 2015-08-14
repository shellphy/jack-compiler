#include "CodeGen.h"
#include <iostream>
#include <sstream>
using namespace std;

//#define fout cout

CodeGen::CodeGen()
{
    ifLabelCount = 0;
    whileLabelCount = 0;
    symbolTable = SymbolTable::getInstance();
}

void CodeGen::write(Parser::TreeNode *t)
{
    static int depth = 0;
    if (depth > 3)
        return;
    while (t != nullptr)
    {
        symbolTable->subroutineTableInsert(t);
        translate(t);
        for (int i = 0; i < 5; i++)
        {
            depth++;
            write(t->child[i]);
            depth--;
        }
        t = t->next;
    }
}

void CodeGen::translate(Parser::TreeNode *t)
{
    switch (t->nodeKind)
    {
    case Parser::CLASS_K:
    {
        fout.close();
        string path = "./api/";
        currentClassName = t->child[0]->token.lexeme;
        if (currentClassName == "Sys" || currentClassName == "Memory" || currentClassName == "Math" ||
                currentClassName == "String" || currentClassName == "Output" || currentClassName == "Input" ||
                currentClassName == "Array" || currentClassName == "IO")
            fout.open(path + currentClassName + ".vm");
        else
            fout.open(currentClassName + ".vm");
        break;
    }
    case Parser::SUBROUTINE_DEC_K:
    {
        isMethod = false;
        currentFunctionName = t->child[2]->token.lexeme;
        int nlocals = 0;
        for (auto p = t->child[4]->child[0]; p != nullptr; p = p->next)
            for (auto q = p->child[1]; q != nullptr; q = q->next)
                nlocals++;
        writeFunction(t->child[2]->token.lexeme, nlocals);
        if (t->child[0]->token.lexeme == "method")
        {
            writePush(ARG, 0);
            writePop(POINTER, 0);
            isMethod = true;
        }
        if (t->child[0]->token.lexeme == "constructor")
        {
            int nFields = symbolTable->getFieldNumber(currentClassName);
            writePush(CONST, nFields);
            writeCall("Memory.alloc", 1);
            writePop(POINTER, 0);
        }
        break;
    }
    case Parser::ASSIGN_K:
    {
        if (t->child[0]->nodeKind == Parser::VAR_K)
        {
            writeExpression(t->child[1]);
            string varName = t->child[0]->token.lexeme;
            SymbolTable::Info info = symbolTable->subroutineTableFind(varName);
            if (info == SymbolTable::None)
                info = symbolTable->classesTableFind(currentClassName, varName);
            if (info.kind == SymbolTable::FIELD)
                writePop(THIS, info.index);
            else if (info.kind == SymbolTable::STATIC)
                writePop(STATIC, info.index);
            else if (info.kind == SymbolTable::ARG)
            {
                if (isMethod)
                    writePop(ARG, info.index + 1);
                else
                    writePop(ARG, info.index);
            }
            else if (info.kind == SymbolTable::VAR)
                writePop(LOCAL, info.index);
        }
        else if (t->child[0]->nodeKind == Parser::ARRAY_K)
        {
            writeExpression(t->child[0]->child[0]);
            string varName = t->child[0]->token.lexeme;
            SymbolTable::Info info = symbolTable->subroutineTableFind(varName);
            if (info == SymbolTable::None)
                info = symbolTable->classesTableFind(currentClassName, varName);
            if (info.kind == SymbolTable::FIELD)
                writePush(THIS, info.index);
            else if (info.kind == SymbolTable::STATIC)
                writePush(STATIC, info.index);
            else if (info.kind == SymbolTable::ARG)
            {
                if (isMethod)
                    writePush(ARG, info.index + 1);
                else
                    writePush(ARG, info.index);
            }
            else if (info.kind == SymbolTable::VAR)
                writePush(LOCAL, info.index);
            writeArithmetic(ADD);
            writeExpression(t->child[1]);
            writePop(TEMP, 0);
            writePop(POINTER, 1);
            writePush(TEMP, 0);
            writePop(THAT, 0);
        }
        break;
    }
    case Parser::CALL_STATEMENT_K:
    {
        translateCall(t);
        writePop(TEMP, 0);
        break;
    }
    case Parser::RETURN_STATEMENT_K:
    {
        if (t->child[0] != nullptr)
            writeExpression(t->child[0]);
        else
            writePush(CONST, 0);
        writeReturn();
        break;
    }
    case Parser::WHILE_STATEMENT_K:
    {
        stringstream ss;
        ss << whileLabelCount;
        whileLabelCount++;
        string whileBegin = "WHILE_EXP" + ss.str();
        string whileEnd = "WHILE_END" + ss.str();

        writeLabel(whileBegin);
        writeExpression(t->child[0]);
        writeArithmetic(NOT);
        writeIf(whileEnd);
        for (auto p = t->child[1]; p != nullptr; p = p->next)
            translate(p);
        writeGoto(whileBegin);
        writeLabel(whileEnd);

        break;
    }
    case Parser::IF_STATEMENT_K:
    {
        stringstream ss;
        ss << ifLabelCount;
        ifLabelCount++;
        string L1 = "IF_TRUE" + ss.str();
        string L2 = "IF_FALSE" + ss.str();
        string L3 = "IF_END" + ss.str();

        writeExpression(t->child[0]);
        writeIf(L1);
        writeGoto(L2);
        writeLabel(L1);
        for (auto p = t->child[1]; p != nullptr; p = p->next)
            translate(p);
        if (t->child[2] == nullptr)
            writeLabel(L2);
        else
        {
            writeGoto(L3);
            writeLabel(L2);
            for (auto p = t->child[2]; p != nullptr; p = p->next)
                translate(p);
            writeLabel(L3);
        }
        break;
    }
    }
}

void CodeGen::translateCall(Parser::TreeNode *t)
{
    if (t->child[0]->nodeKind == Parser::METHOD_CALL_K)
    {
        if (t->token.lexeme.find('.') == string::npos)     // method() 调用
        {
            writePush(POINTER, 0);
            t->token.lexeme = currentClassName + "." + t->token.lexeme;
        }
        else                         // obj.method()调用
        {
            string objName = Parser::getCallerName(t->token.lexeme);
            SymbolTable::Info info = symbolTable->subroutineTableFind(objName);
            if (info == SymbolTable::None)
                info = symbolTable->classesTableFind(currentClassName, objName);
            if (info.kind == SymbolTable::FIELD)
                writePush(THIS, info.index);
            else if (info.kind == SymbolTable::VAR)
                writePush(LOCAL, info.index);
            else if (info.kind == SymbolTable::ARG)
                writePush(ARG, info.index);
            else if (info.kind == SymbolTable::STATIC)
                writePush(STATIC, info.index);
            string functionName = Parser::getFunctionName(t->token.lexeme);
            t->token.lexeme = info.type + "." + functionName;
        }
    }
    int nArgs = 0;
    for (auto p = t->child[0]->next; p != nullptr; p = p->next)
    {
        writeExpression(p);
        nArgs++;
    }
    if (t->child[0]->nodeKind == Parser::METHOD_CALL_K)
        nArgs++;
    writeCall(t->token.lexeme, nArgs);
}

// 后序遍历
void CodeGen::writeExpression(Parser::TreeNode *t)
{
    if (t != nullptr)
    {
        for (int i = 0; i < 5; i++)
            writeExpression(t->child[i]);
        switch (t->nodeKind)
        {
        case Parser::OPERATION_K:
            if (t->token.lexeme == "+")
                writeArithmetic(ADD);
            else if (t->token.lexeme == "-")
                writeArithmetic(SUB);
            else if (t->token.lexeme == "*")
                writeCall("Math.multiply", 2);
            else if (t->token.lexeme == "/")
                writeCall("Math.divide", 2);
            break;
        case Parser::COMPARE_K:
        {
            if (t->token.lexeme == ">")
                writeArithmetic(GT);
            else if (t->token.lexeme == "<")
                writeArithmetic(LT);
            else if (t->token.lexeme == "==")
                writeArithmetic(EQ);
            else if (t->token.lexeme == "<=")
            {
                writeArithmetic(GT);
                writeArithmetic(NOT);
            }
            else if (t->token.lexeme == ">=")
            {
                writeArithmetic(LT);
                writeArithmetic(NOT);
            }
            else if (t->token.lexeme == "!=")
                writeArithmetic(NOT);
            break;
        }
        case Parser::BOOL_EXPRESSION_K:
        {
            if (t->token.lexeme == "&")
                writeArithmetic(AND);
            else if (t->token.lexeme == "|")
                writeArithmetic(OR);
            else if (t->token.lexeme == "~")
                writeArithmetic(NOT);
            break;
        }
        case Parser::INT_CONST_K:
            writePush(CONST, atoi(t->token.lexeme.c_str()));
            break;
        case Parser::NEGATIVE_K:
            writeArithmetic(NEG);
            break;
        case Parser::CALL_EXPRESSION_K:
        {
            translateCall(t);
            break;
        }
        case Parser::VAR_K:
        {
            string varName = t->token.lexeme;
            SymbolTable::Info info = symbolTable->subroutineTableFind(varName);
            if (info == SymbolTable::None)
                info = symbolTable->classesTableFind(currentClassName, varName);
            if (info.kind == SymbolTable::FIELD)
                writePush(THIS, info.index);
            else if (info.kind == SymbolTable::STATIC)
                writePush(STATIC, info.index);
            else if (info.kind == SymbolTable::ARG)
            {
                if (isMethod)
                    writePush(ARG, info.index + 1);
                else
                    writePush(ARG, info.index);
            }
            else if (info.kind == SymbolTable::VAR)
                writePush(LOCAL, info.index);
            break;
        }
        case Parser::ARRAY_K:
        {
            string varName = t->token.lexeme;
            SymbolTable::Info info = symbolTable->subroutineTableFind(varName);
            if (info == SymbolTable::None)
                info = symbolTable->classesTableFind(currentClassName, varName);
            if (info.kind == SymbolTable::FIELD)
                writePush(THIS, info.index);
            else if (info.kind == SymbolTable::STATIC)
                writePush(STATIC, info.index);
            else if (info.kind == SymbolTable::ARG)
                writePush(ARG, info.index);
            else if (info.kind == SymbolTable::VAR)
                writePush(LOCAL, info.index);
            writeArithmetic(ADD);
            writePop(POINTER, 1);
            writePush(THAT, 0);
            break;
        }
        case Parser::BOOL_CONST_K:
        {
            writePush(CONST, 0);
            if (t->token.lexeme == "true")
                writeArithmetic(NOT);
            break;
        }
        case Parser::THIS_K:
        {
            writePush(POINTER, 0);
            break;
        }
        case Parser::NULL_K:
        {
            writePush(CONST, 0);
            break;
        }
        case Parser::STRING_CONST_K:
        {
            int stringlength = t->token.lexeme.length();
            writePush(CONST, stringlength);
            writeCall("String.new", 1);
            for (int i = 0; i < stringlength; i++)
            {
                writePush(CONST, t->token.lexeme[i]);
                writeCall("String.appendChar", 2);
            }
            break;
        }
        }
    }
    
}

void CodeGen::writePush(Segment segment, int index)
{
    switch (segment)
    {
    case CONST:
        fout << "push constant " << index << "\n";
        break;
    case ARG:
        fout << "push argument " << index << "\n";
        break;
    case LOCAL:
        fout << "push local " << index << "\n";
        break;
    case STATIC:
        fout << "push static " << index << "\n";
        break;
    case THIS:
        fout << "push this " << index << "\n";
        break;
    case THAT:
        fout << "push that " << index << "\n";
        break;
    case POINTER:
        fout << "push pointer " << index << "\n";
        break;
    case TEMP:
        fout << "push temp " << index << "\n";
        break;
    }
}

void CodeGen::writePop(Segment segment, int index)
{
    switch (segment)
    {
    case CONST:
        fout << "pop constant " << index << "\n";
        break;
    case ARG:
        fout << "pop argument " << index << "\n";
        break;
    case LOCAL:
        fout << "pop local " << index << "\n";
        break;
    case STATIC:
        fout << "pop static " << index << "\n";
        break;
    case THIS:
        fout << "pop this " << index << "\n";
        break;
    case THAT:
        fout << "pop that " << index << "\n";
        break;
    case POINTER:
        fout << "pop pointer " << index << "\n";
        break;
    case TEMP:
        fout << "pop temp " << index << "\n";
        break;
    }
}

void CodeGen::writeArithmetic(Command command)
{
    switch(command)
    {
    case ADD:
        fout << "add\n";
        break;
    case SUB:
        fout << "sub\n";
        break;
    case NEG:
        fout << "neg\n";
        break;
    case EQ:
        fout << "eq\n";
        break;
    case GT:
        fout << "gt\n";
        break;
    case LT:
        fout << "lt\n";
        break;
    case AND:
        fout << "and\n";
        break;
    case OR:
        fout << "or\n";
        break;
    case NOT:
        fout << "not\n";
        break;
    }
}

void CodeGen::writeLabel(string label)
{
    fout << "label " << label << "\n";
}

void CodeGen::writeGoto(string label)
{
    fout << "goto " << label << "\n";
}

void CodeGen::writeIf(string label)
{
    fout << "if-goto " << label << "\n";
}

void CodeGen::writeCall(string name, int nArgs)
{
    fout << "call " << name << " " << nArgs << "\n";
}

void CodeGen::writeFunction(string name, int nArgs)
{
    fout << "function " << name << " " << nArgs << "\n";
}

void CodeGen::writeReturn()
{
    fout << "return\n";
}
