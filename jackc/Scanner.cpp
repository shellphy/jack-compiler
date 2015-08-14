#include "Scanner.h"
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <iterator>

using namespace std;

Scanner::Scanner()
{
    row = 0;
    bufferPos = 0;
    initKeyWords();
    initSymbols();
}

void Scanner::resetRow()
{
    row = 0;
}

void Scanner::openFile(string filename)
{
    string suffix = filename.substr(filename.size() - 5, 5);
    if (suffix != ".jack")
    {
        cerr <<  "file '"<<filename << "' is not a standard java-- file, filename must be ended by '.j' or '.J'" << endl;
        exit(-1);
    }
    fin.open(filename);
    if (fin.fail())
    {
        cerr << "file " << filename << " does not exit" << endl;
        exit(-1);
    }
}

void Scanner::closeFile()
{
    fin.close();
}

void Scanner::initKeyWords()
{
    keyWords.insert("class");
    keyWords.insert("constructor");
    keyWords.insert("function");
    keyWords.insert("method");
    keyWords.insert("field");
    keyWords.insert("static");
    keyWords.insert("int");
    keyWords.insert("char");
    keyWords.insert("boolean");
    keyWords.insert("void");
    keyWords.insert("true");
    keyWords.insert("false");
//    keyWords.insert("null");
    keyWords.insert("this");
    keyWords.insert("if");
    keyWords.insert("else");
    keyWords.insert("while");
    keyWords.insert("return");
}

void Scanner::initSymbols()
{
    symbols.insert("{");
    symbols.insert("}");
    symbols.insert("(");
    symbols.insert(")");
    symbols.insert("[");
    symbols.insert("]");
    symbols.insert(".");
    symbols.insert(",");
    symbols.insert(";");
    symbols.insert("+");
    symbols.insert("-");
    symbols.insert("*");
    symbols.insert("/");
    symbols.insert("&");
    symbols.insert("|");
    symbols.insert("~");
    symbols.insert("<");
    symbols.insert(">");
    symbols.insert("=");
    symbols.insert(">=");
    symbols.insert("<=");
    symbols.insert("==");
    symbols.insert("!=");
//    symbols.insert("!");
//    symbols.insert("&&");
//    symbols.insert("||");
}

Scanner::TokenType Scanner::searchReserved(string &s)
{
    if (keyWords.find(s) != keyWords.end())
        return KEY_WORD;
    else
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
    {
        return lineBuffer[bufferPos++];
    }
}

void Scanner::rollBack()
{
    assert(bufferPos > 0);
    bufferPos--;
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
            token.kind = ENDOFFILE;
            break;
        }
        switch (state)
        {
        case START_STATE:										// ¿ªÊ¼×´Ì¬
            if (ch == ' ' || ch == '\t' || ch == '\n')
                ;
            else if (isalpha(ch) || ch == '_')
            {
                state = ID_STATE;		// ½øÈë±êÊ¶·û×´Ì¬
                token.kind = ID;
                token.lexeme += ch;
                token.row = row;
            }
            else if (isdigit(ch))
            {
                state = INT_STATE;		// ½øÈëÕûÊý×´Ì¬
                token.kind = INT;
                token.lexeme += ch;
                token.row = row;
            }

            else if (symbols.find({ch}) != symbols.end())
            {
                state = SYMBOL_STATE;
                token.kind = SYMBOL;
                token.lexeme += ch;
                token.row = row;
            }
            else if (ch == '"')
            {
                state = STRING_STATE;	// ½øÈë×Ö·û´®×´Ì¬
                token.kind = STRING;
                token.row = row;
            }
            else if (ch == '\'')		// ½øÈëµ¥×Ö·û×´Ì¬
            {
                state = CHAR_STATE;
                token.kind = CHAR;
                token.row = row;
            }
            else												// ÆäËü·Ç·¨×Ö·û
            {
                state = ERROR_STATE;
                token.kind = ERROR;
                token.lexeme += ch;
                token.row = row;
            }
            break;
        case INT_STATE:											// ÕûÊý×´Ì¬
            if (isdigit(ch))
            {
                token.lexeme += ch;
            }
            else
            {
                rollBack();
                state = DONE_STATE;
            }
            break;
        case ID_STATE:											// ±êÊ¶·û×´Ì¬
            if (isalpha(ch) || isdigit(ch) || ch == '_')
            {
                token.lexeme += ch;
            }
            else
            {
                rollBack();
                state = DONE_STATE;
            }
            break;
        case STRING_STATE:										// ×Ö·û´®×´Ì¬
            if (ch == '"')
            {
                state = DONE_STATE;
            }
            else if (ch == '\\')
            {
                state = S_STRING_STATE;
                token.lexeme += ch;
            }
            else
            {
                token.lexeme += ch;
            }
            break;
        case S_STRING_STATE:
            state = STRING_STATE;
            token.lexeme.pop_back();
            token.lexeme += ch;
            break;
        case CHAR_STATE:										// ×Ö·û×´Ì¬
            if (ch != '\\' && ch != '\'')
            {
                state = CHAR_STATE_A;
                token.lexeme += ch;
            }
            else if (ch == '\\')
            {
                state = CHAR_STATE_B;
                token.lexeme += ch;
            }
            else if (ch == '\'')
            {
                state = ERROR_STATE;
                token.kind = ERROR;
                token.lexeme += ch;
                token.lexeme += ch;
            }
            break;
        case SYMBOL_STATE:
            if (token.lexeme == "/")
            {
                if (ch == '*')
                {
                    state = INCOMMENT_STATE;
                    token.lexeme.pop_back();
                }
                else if (ch == '/')
                {
                    state = START_STATE;
                    bufferPos = lineBuffer.length();
                    token.lexeme.pop_back();
                }
                else
                {
                    rollBack();
                    state = DONE_STATE;
                }
            }
            else if (token.lexeme == "<")
            {
                if (ch == '=')
                {
                    token.lexeme += ch;
                    state = DONE_STATE;
                }
                else
                {
                    rollBack();
                    state = DONE_STATE;
                }
            }
            else if (token.lexeme == "=")
            {
                if (ch == '=')
                {
                    token.lexeme += ch;
                    state = DONE_STATE;
                }
                else
                {
                    rollBack();
                    state = DONE_STATE;
                }
            }
            else if (token.lexeme == "!")
            {
                if (ch == '=')
                {
                    token.lexeme += ch;
                    state = DONE_STATE;
                }
                else
                {
                    rollBack();
                    state = DONE_STATE;
                }
            }
            else if (token.lexeme == "&")
            {
                if (ch == '&')
                {
                    token.lexeme += ch;
                    state = DONE_STATE;
                }
                else
                {
                    rollBack();
                    state = DONE_STATE;
                }
            }
            else if (token.lexeme == "|")
            {
                if (ch == '|')
                {
                    token.lexeme += ch;
                    state = DONE_STATE;
                }
                else
                {
                    rollBack();
                    state = DONE_STATE;
                }
            }
            else
            {
                rollBack();
                state = DONE_STATE;
            }
            break;
        case CHAR_STATE_A:
            if (ch == '\'')
            {
                state = DONE_STATE;
            }
            else
            {
                state = ERROR_STATE;
                token.kind = ERROR;
                string tmp = "'";
                tmp.append(token.lexeme);
                tmp += ch;
                token.lexeme = tmp;
            }
            break;
        case CHAR_STATE_B:
            if (ch == 'a' || ch == 'b' || ch == 'f' || ch == 'n' || ch == 'r' || ch == 't' || ch == 'v'
                || ch == '\\' || ch == '?' || ch == '\'' || ch == '"')
            {
                state = CHAR_STATE_C;
                token.lexeme += ch;
            }
            else
            {
                state = ERROR_STATE;
                token.kind = ERROR;
                string tmp = "'\\";
                tmp += ch;
                token.lexeme = tmp;
            }
            break;
        case CHAR_STATE_C:
            if (ch == '\'')
            {
                state = DONE_STATE;
            }
            else
            {
                state = ERROR_STATE;
                token.kind = ERROR;
                string tmp = "'";
                tmp.append(token.lexeme);
                tmp += ch;
                token.lexeme = tmp;
            }
            break;
        case ERROR_STATE:										// ´íÎó×´Ì¬
            if (ch == ' ' || ch == '\n' || ch == '\t')
                state = DONE_STATE;
            else
                token.lexeme += ch;
            break;
        case INCOMMENT_STATE:									// ×¢ÊÍ×´Ì¬
            if (ch == '*')
                state = P_INCOMMENT_STATE;
            break;
        case P_INCOMMENT_STATE:									// ½Ó½üÍË³ö×¢ÊÍ×´Ì¬
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
