#include "Parser.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

Parser::Parser(vector<string> & filenames)
{
    this->filenames = filenames;
    syntaxTree = nullptr;
}

Scanner::Token Parser::getToken()
{
    if (tokenBuffer2.size() == 0)
    {
        auto token = scanner.nextToken();
        tokenBuffer1.push_back(token);
        if (tokenBuffer1.size() > 10)
            tokenBuffer1.pop_front();
    }
    else
    {
        auto token = tokenBuffer2.front();
        tokenBuffer1.push_back(token);
        tokenBuffer2.pop_front();
    }
    return tokenBuffer1.back();
}

Scanner::Token Parser::ungetToken()
{
    assert(tokenBuffer1.size() > 0);
    auto token = tokenBuffer1.back();
    tokenBuffer2.push_front(token);
    tokenBuffer1.pop_back();
    return tokenBuffer1.back();
}

string Parser::getFullName(string name)
{
    string fullname = currentParserFilename + "." + name;
    return fullname;
}

string Parser::getCallerName(string fullName)
{
    auto iter = fullName.cbegin();
    while (iter != fullName.cend())
    {
        if (*iter == '.')
            break;
        ++iter;
    }
    return string(fullName.cbegin(), iter);
}

string Parser::getFunctionName(string fullName)
{
    auto iter = fullName.cbegin();
    while (iter != fullName.cend())
    {
        if (*iter == '.')
            break;
        ++iter;
    }
    return string(++iter, fullName.cend());
}

void Parser::parse_program()
{
    syntaxTree = parse_class_list();
}

Parser::TreeNode * Parser::parse_class_list()
{
    TreeNode * t = nullptr;
    TreeNode * p = nullptr;
    for (auto filenameIter = filenames.cbegin(); filenameIter != filenames.cend(); ++filenameIter)
    {
        scanner.openFile(*filenameIter);
        auto classNameIter = filenameIter->rbegin();
        int begin = 0;
        while (classNameIter != filenameIter->rend())
        {
            if (*classNameIter == '/')
                break;
            begin++;
            ++classNameIter;
        }
        currentParserFilename = filenameIter->substr(filenameIter->size() - begin, begin - 5);
        scanner.resetRow();
        TreeNode * q = parse_class();
        if (getToken().kind != Scanner::ENDOFFILE)
            cerr << "Syntax Error in class " << currentParserFilename << ": unexpected token before EOF " << endl;
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
        scanner.closeFile();
    }
    return t;
}

Parser::TreeNode * Parser::parse_class()
{
    TreeNode * t = new TreeNode;
    Scanner::Token token = getToken();
    t->nodeKind = CLASS_K;
    token = getToken();
    if (token.kind != Scanner::ID)
    {
        syntaxError(currentParserFilename, "identifier", token);
        return t;
    }
    t->child[0] = new TreeNode;
    t->child[0]->token = token;
    if (currentParserFilename != token.lexeme)
    {
        error1(currentParserFilename);
        return t;
    }
    token = getToken();
    if (token.lexeme != "{")
    {
        syntaxError(currentParserFilename, "{", token);
        return t;
    }
    t->child[1] = parse_class_var_dec_list();
    t->child[2] = parse_subroutine_dec_list();
    token = getToken();
    if (token.lexeme != "}")
    {
        syntaxError(currentParserFilename, "}", token);
        return t;
    }
    return t;
}

Parser::TreeNode * Parser::parse_class_var_dec_list()
{
    TreeNode * t = nullptr;
    TreeNode * p = t;
    auto token = getToken();
    while (token.lexeme == "static" || token.lexeme == "field")
    {
        ungetToken();
        TreeNode * q = parse_class_var_dec();
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
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_class_var_dec()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = CLASS_VAR_DEC_K;
    Scanner::Token token = getToken();
    if (token.lexeme != "static" && token.lexeme != "field")
    {
        syntaxError(currentParserFilename, "static or filed", token);
        return t;
    }
    t->child[0] = new TreeNode;
    t->child[0]->token.lexeme = token.lexeme;
    t->child[1] = parse_type();
    t->child[2] = parse_var_name_list();
    token = getToken();
    if (token.lexeme != ";")
    {
        syntaxError(currentParserFilename, ";", token);
        return t;
    }
    return t;
}

Parser::TreeNode * Parser::parse_var_name_list()
{
    TreeNode * t = new TreeNode;
    Scanner::Token token = getToken();
    if (token.kind != Scanner::ID)
    {
        syntaxError(currentParserFilename, "identifier", token);
        return t;
    }
    t->token = token;
    TreeNode * p = t;
    token = getToken();
    while (token.lexeme == ",")
    {
        token = getToken();
        if (token.kind != Scanner::ID)
        {
            syntaxError(currentParserFilename, "identifier", token);
            return t;
        }
        TreeNode * q = new TreeNode;
        q->token = token;
        p->next = q;
        p = q;
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_type()
{
    TreeNode * t = nullptr;
    Scanner::Token token = getToken();
    if (token.kind == Scanner::ID)
    {
        t = new TreeNode;
        t->nodeKind = CLASS_TYPE_K;
        t->token.lexeme = token.lexeme;
    }
    else if (token.lexeme == "int" || token.lexeme == "char"
        || token.lexeme == "boolean" || token.lexeme == "void")
    {
        t = new TreeNode;
        t->nodeKind = BASIC_TYPE_K;
        t->token.lexeme = token.lexeme;
    }
    else
    {
        syntaxError(currentParserFilename, "basic type or class type", token);
        return t;
    }
    return t;
}

Parser::TreeNode * Parser::parse_subroutine_dec_list()
{
    TreeNode * t = nullptr;
    TreeNode * p = t;
    auto token = getToken();
    while (token.lexeme == "constructor" || token.lexeme == "function" || token.lexeme == "method")
    {
        ungetToken();
        TreeNode * q = parse_subroutin_dec();
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
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_subroutin_dec()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = SUBROUTINE_DEC_K;
    Scanner::Token token = getToken();
    if (token.lexeme == "constructor" || token.lexeme == "function" || token.lexeme == "method")
    {
        t->child[0] = new TreeNode;
        t->child[0]->token = token;
    }
    else
    {
        syntaxError(currentParserFilename, "constructor or function or method", token);
        return t;
    }
    t->child[1] = parse_type();
    token = getToken();
    if (token.kind == Scanner::ID)
    {
        t->child[2] = new TreeNode;
        t->child[2]->token = token;
        t->child[2]->token.lexeme = getFullName(token.lexeme);        
    }
    else
    {
        syntaxError(currentParserFilename, "identifile", token);
        return t;
    }

    token = getToken();
    if (token.lexeme != "(")
    {
        syntaxError(currentParserFilename, "(", token);
        return t;
    }
    t->child[3] = parse_params();
    token = getToken();
    if (token.lexeme != ")")
    {
        syntaxError(currentParserFilename, ")", token);
        return t;
    }
    t->child[4] = parse_subroutine_body();
    return t;
}

Parser::TreeNode * Parser::parse_params()
{
    TreeNode * t = nullptr;
    Scanner::Token token = getToken();
    if (token.lexeme != ")")
    {
        ungetToken();
        t = parse_param_list();
    }
    else
        ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_param_list()
{
    TreeNode * t = parse_param();
    TreeNode * p = t;
    Scanner::Token token = getToken();
    while (token.lexeme == ",")
    {
        TreeNode * q = parse_param();
        p->next = q;
        p = q;
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_param()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = PARAM_K;
    t->child[0] = parse_type();
    Scanner::Token token = getToken();
    if (token.kind == Scanner::ID)
    {
        t->child[1] = new TreeNode;
        t->child[1]->token = token;
    }
    else
    {
        syntaxError(currentParserFilename, "identifier", token);
        return t;
    }
    return t;
}

Parser::TreeNode * Parser::parse_subroutine_body()
{
    haveReturnStatement = false;

    TreeNode * t = new TreeNode;
    t->nodeKind = SUBROUTINE_BODY_K;
    Scanner::Token token = getToken();
    if (token.lexeme != "{")
    {
        syntaxError(currentParserFilename, "{", token);
        return t;
    }
    t->child[0] = parse_var_dec_list();
    t->child[1] = parse_statements();
    
    token = getToken();
    if (token.lexeme != "}")
    {
        syntaxError(currentParserFilename, "}", token);
        return t;
    }
    if (haveReturnStatement == false)
    {
        syntaxError(currentParserFilename, "return statement", token);
        return t;
    }
    return t;
}

Parser::TreeNode * Parser::parse_var_dec_list()
{
    TreeNode * t = nullptr;
    TreeNode * p = t;

Loop:
    Scanner::Token token = getToken();
    if (token.lexeme == "int" || token.lexeme == "char"
        || token.lexeme == "boolean" || token.lexeme == "string")
    {
        ungetToken();
        TreeNode * q = parse_var_dec();
        if (t == nullptr)
            t = p = q;
        else
        {
            p->next = q;
            p = q;
        }  
        goto Loop;
    }
    else if (token.kind == Scanner::ID)     // 类变量声明
    {
        token = getToken();
        if (token.kind == Scanner::ID)
        {
            ungetToken();
            ungetToken();
            TreeNode * q = parse_var_dec();
            if (t == nullptr)
                t = p = q;
            else
            {
                p->next = q;
                p = q;
            }
            goto Loop;
        }
        ungetToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_var_dec()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = VAR_DEC_K;
    Scanner::Token token;
    t->child[0] = parse_type();
    t->child[1] = parse_var_name_list();
    token = getToken();
    if (token.lexeme != ";")
    {
        syntaxError(currentParserFilename, ";", token);
        return t;
    }
    return t;
}

Parser::TreeNode * Parser::parse_statements()
{
    TreeNode * t = nullptr;
    TreeNode * p = t;
    Scanner::Token token = getToken();
    while (token.lexeme == "if" || token.lexeme == "while" || token.lexeme == "return"
        || token.kind == Scanner::ID)
    {
        if (token.lexeme == "return")
            haveReturnStatement = true;
        if (token.kind == Scanner::ID)
        {
            token = getToken();
            if (token.lexeme == "=" || token.lexeme == "[" || token.lexeme == "(" || token.lexeme == ".")
            {
                ungetToken();
                ungetToken();
                TreeNode * q = parse_statement();
                if (t == nullptr)
                    t = p = q;
                else
                {
                    p->next = q;
                    p = q;
                }
            }
            else
            {
                ungetToken();
                break;
            }
        }
        else
        {
            ungetToken();
            TreeNode * q = parse_statement();
            if (t == nullptr)
                t = p = q;
            else
            {
                p->next = q;
                p = q;
            }
        }
        token = getToken();
    }
    ungetToken();

    return t;
}

Parser::TreeNode * Parser::parse_statement()
{
    TreeNode * t = nullptr;
    Scanner::Token token = getToken();
    if (token.lexeme == "if")
    {
        ungetToken();
        t = parse_if_statement();
    }
    else if (token.lexeme == "while")
    {
        ungetToken();
        t = parse_while_statement();
    }
    else if (token.lexeme == "return")
    {
        ungetToken();
        t = parse_return_statement();
    }
    else if (token.kind == Scanner::ID)
    {
        token = getToken();
        if (token.lexeme == "=" || token.lexeme == "[")
        {
            ungetToken();
            ungetToken();
            t = parse_assign_statement();
        
        }
        else if (token.lexeme == "(" || token.lexeme == ".")
        {
            ungetToken();
            ungetToken();
            t = parse_call_statement();
            token = getToken();
            if (token.lexeme != ";")
            {
                ungetToken();
                syntaxError(currentParserFilename, ";", token);
                return t;
            }
        }
        else
        {
            ungetToken();
            ungetToken();
            syntaxError(currentParserFilename, "'=' or '[' or '(' or '.'", token);
            return t;
        }
    }
    else
    {
        ungetToken();
        syntaxError(currentParserFilename, "identifier", token);
        return t;
    }
    return t;
}

Parser::TreeNode * Parser::parse_assign_statement()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = ASSIGN_K;
    t->child[0] = parse_left_value();
    Scanner::Token token = getToken();
    t->child[1] = parse_expression();
    token = getToken();
    if (token.lexeme != ";")
    {
        syntaxError(currentParserFilename, ";", token);
        return t;
    }
    return t;
}

Parser::TreeNode * Parser::parse_left_value()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = VAR_K;
    Scanner::Token token = getToken();
    t->token = token;
    token = getToken();
    if (token.lexeme == "[")
    {
        t->nodeKind = ARRAY_K;
        t->child[0] = parse_expression();
        token = getToken();
        if (token.lexeme != "]")
        {
            syntaxError(currentParserFilename, "]", token);
            return t;
        }
        t->nodeKind = ARRAY_K;
    }
    else if (token.lexeme == "=")
    {
        ungetToken();
    }
    return t;
}

Parser::TreeNode * Parser::parse_if_statement()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = IF_STATEMENT_K;
    Scanner::Token token = getToken();
    token = getToken();
    if (token.lexeme != "(")
    {
        syntaxError(currentParserFilename, "(", token);
        return t;
    }
    t->child[0] = parse_expression();
    token = getToken();
    if (token.lexeme != ")")
    {
        syntaxError(currentParserFilename, ")", token);
        return t;
    }
    token = getToken();
    if (token.lexeme != "{")
    {
        syntaxError(currentParserFilename, "{", token);
        return t;
    }
    t->child[1] = parse_statements();
    token = getToken();
    if (token.lexeme != "}")
    {
        syntaxError(currentParserFilename, "}", token);
        return t;
    }
    token = getToken();
    if (token.lexeme == "else")
    {
        token = getToken();
        if (token.lexeme != "{")
        { 
            syntaxError(currentParserFilename, "{", token);
            return t;
        }
        t->child[2] = parse_statements();
        token = getToken();
        if (token.lexeme != "}")
        {
            syntaxError(currentParserFilename, "}", token);
            return t;
        }
    }
    else
        ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_while_statement()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = WHILE_STATEMENT_K;
    Scanner::Token token = getToken();
    token = getToken();
    if (token.lexeme != "(")
    {
        syntaxError(currentParserFilename, "(", token);
        return t;
    }
    t->child[0] = parse_expression();
    token = getToken();
    if (token.lexeme != ")")
    {
        syntaxError(currentParserFilename, ")", token);
        return t;
    }
    token = getToken();
    if (token.lexeme != "{")
    {
        syntaxError(currentParserFilename, "{", token);
        return t;
    }
    t->child[1] = parse_statements();
    token = getToken();
    if (token.lexeme != "}")
    {
        syntaxError(currentParserFilename, "}", token);
        return t;
    }
    return t;
}

Parser::TreeNode * Parser::parse_return_statement()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = RETURN_STATEMENT_K;
    Scanner::Token token = getToken();
    t->token = token;
    token = getToken();
    if (token.lexeme == ";")
        return t;
    else
    {
        ungetToken();
        t->child[0] = parse_expression();
        token = getToken();
        if (token.lexeme != ";")
        {
            syntaxError(currentParserFilename, ";", token);
            return t;
        }
    }
    return t;
}

Parser::TreeNode * Parser::parse_call_statement()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = CALL_STATEMENT_K;
    Scanner::Token token = getToken();
    Scanner::Token save = token;
    t->child[0] = new TreeNode;
    token = getToken();
    if (token.lexeme == "(")
    {
        t->child[0]->next = parse_expressions();
        token = getToken();
        if (token.lexeme != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    }
    else if (token.lexeme == ".")
    {
        token = getToken();
        if (token.kind != Scanner::ID)
        {
            syntaxError(currentParserFilename, "identifier", token);
            return t;
        }
        save.lexeme = save.lexeme + "." + token.lexeme;
        token = getToken();
        if (token.lexeme != "(")
        {
            syntaxError(currentParserFilename, "(", token);
            return t;
        }
        t->child[0]->next = parse_expressions();
        token = getToken();
        if (token.lexeme != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    }
    t->token = save;
    return t;
}

Parser::TreeNode * Parser::parse_expressions()
{
    TreeNode * t = nullptr;
    Scanner::Token token = getToken();
    if (token.lexeme == ")")
    {
        ungetToken();
        return t;
    }
    else
    {
        ungetToken();
        t = parse_expression_list();
        return t;
    }
}

Parser::TreeNode * Parser::parse_expression_list()
{
    TreeNode * t = parse_expression();
    TreeNode * p = t;
    Scanner::Token token = getToken();
    while (token.lexeme == ",")
    {
        TreeNode * q = parse_expression();
        p->next = q;
        p = q;
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_expression()
{
    TreeNode * t = parse_bool_expression();
    Scanner::Token token = getToken();
    while (token.lexeme == "&" || token.lexeme == "|")
    {
        TreeNode * p = new TreeNode();
        p->nodeKind = BOOL_EXPRESSION_K;
        p->token = token;
        p->child[0] = t;
        t = p;
        t->child[1] = parse_bool_expression();
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_bool_expression()
{
    TreeNode * t = parse_additive_expression();
    Scanner::Token token = getToken();
    if (token.lexeme == "<=" || token.lexeme == ">=" || token.lexeme == "=="
        || token.lexeme == "<" || token.lexeme == ">" || token.lexeme == "!=")
    {
        TreeNode * p = new TreeNode;
        p->nodeKind = COMPARE_K;
        p->token = token;
        p->child[0] = t;
        t = p;
        t->child[1] = parse_additive_expression();
    }
    else
        ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_additive_expression()
{
    TreeNode * t = parse_term();
    Scanner::Token token = getToken();
    while (token.lexeme == "+" || token.lexeme == "-")
    {
        TreeNode * p = new TreeNode;
        p->nodeKind = OPERATION_K;
        p->token = token;
        p->child[0] = t;
        t = p;
        p->child[1] = parse_term();
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_term()
{
    TreeNode * t = parse_factor();
    Scanner::Token token = getToken();
    while (token.lexeme == "*" || token.lexeme == "/")
    {
        TreeNode * p = new TreeNode;
        p->nodeKind = OPERATION_K;
        p->token = token;
        p->child[0] = t;
        t = p;
        p->child[1] = parse_factor();
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode * Parser::parse_factor()
{
    TreeNode * t = nullptr;
    Scanner::Token token = getToken();
    if (token.lexeme == "-")
    {
        t = new TreeNode;
        t->nodeKind = NEGATIVE_K;
        t->token = token;
        t->child[0] = parse_positive_factor();
    }
    else
    {
        ungetToken();
        t = parse_positive_factor();
    }
    return t;
}

Parser::TreeNode * Parser::parse_positive_factor()
{
    TreeNode * t = nullptr;
    Scanner::Token token = getToken();
    if (token.lexeme == "~")
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = BOOL_EXPRESSION_K;
        t->child[0] = parse_not_factor();
    }
    else
    {
        ungetToken();
        t = parse_not_factor();
    }
    return t;
}

Parser::TreeNode * Parser::parse_not_factor()
{
    TreeNode * t = nullptr;
    Scanner::Token token = getToken();
    if (token.lexeme == "(")
    {
        t = parse_expression();
        token = getToken();
        if (token.lexeme != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    }
    else if (token.kind == Scanner::INT)
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = INT_CONST_K;
    }
    else if (token.kind == Scanner::CHAR)
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = CHAR_CONST_K;
    }
    else if (token.kind == Scanner::STRING)
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = STRING_CONST_K;
    }
    else if (token.lexeme == "true" || token.lexeme == "false")
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = BOOL_CONST_K;
    }
    else if (token.lexeme == "this")
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = THIS_K;
    }
    else if (token.lexeme == "null")
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = NULL_K;
    }
    else if (token.kind == Scanner::ID)
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = VAR_K;
        token = getToken();
        if (token.lexeme == "[")
        {
            TreeNode * p = parse_expression();
            t->child[0] = p;
            token = getToken();
            if (token.lexeme != "]")
            {
                syntaxError(currentParserFilename, "]", token);
                return t;
            }
            t->nodeKind = ARRAY_K;
        }
        else if (token.lexeme == "(" || token.lexeme == ".")
        {
            ungetToken();
            ungetToken();
            t = parse_call_expression();
        }
        else
            ungetToken();
    }
    return t;
}

Parser::TreeNode * Parser::parse_call_expression()
{
    TreeNode * t = new TreeNode;
    t->nodeKind = CALL_EXPRESSION_K;
    Scanner::Token token = getToken();
    Scanner::Token save = token;
    t->child[0] = new TreeNode;
    token = getToken();
    if (token.lexeme == "(")
    {
        t->child[0]->next = parse_expressions();
        token = getToken();
        if (token.lexeme != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    }
    else if (token.lexeme == ".")
    {
        token = getToken();
        if (token.kind != Scanner::ID)
        {
            syntaxError(currentParserFilename, "identifier", token);
            return t;
        }
        save.lexeme = save.lexeme + "." + token.lexeme;
        token = getToken();
        if (token.lexeme != "(")
        {
            syntaxError(currentParserFilename, "(", token);
            return t;
        }
        t->child[0]->next = parse_expressions();
        token = getToken();
        if (token.lexeme != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    }
    t->token = save;
    return t;
}

void Parser::print()
{
    printSyntaxTree(syntaxTree);
}

void Parser::printSyntaxTree(TreeNode *tree)
{
    static int indentno = 0;
    indentno += 2;
    while (tree != nullptr)
    {
        for (int i = 0; i < indentno; i++)
            cout << " ";
        switch (tree->nodeKind)
        {
        case CLASS_K:
            cout << "class" << endl;
            break;
        case CLASS_VAR_DEC_K:
            cout << "class_var_dec" << endl;
            break;
        case SUBROUTINE_DEC_K:
            cout << "subroutine_dec" << endl;
            break;
        case BASIC_TYPE_K: 
            cout << "basic_type " << tree->token.lexeme << endl;
            break;
        case CLASS_TYPE_K:
            cout << "class_type " << tree->token.lexeme << endl;
            break;
        case PARAM_K:
            cout << "param" << endl;
            break;
        case VAR_DEC_K:
            cout << "var_dec" << endl;
            break;
        case ARRAY_K:
            cout << "array" << endl;
            break;
        case VAR_K:
            cout << "var" << endl;
            break;
        case IF_STATEMENT_K:
            cout << "if_statement" << endl;
            break;
        case WHILE_STATEMENT_K:
            cout << "while_statement" << endl;
            break;
        case RETURN_STATEMENT_K:
            cout << "return_statement" << endl;
            break;
        case CALL_STATEMENT_K:
            cout << "call_statement" << endl;
            break;
        case BOOL_EXPRESSION_K:
            cout << "bool_expression " << tree->token.lexeme << endl;
            break;
        case COMPARE_K:
            cout << "compare " << tree->token.lexeme << endl;
            break;
        case OPERATION_K:
            cout << "operation " << tree->token.lexeme <<  endl;
            break;
        case BOOL_K:
            cout << "bool" << endl;
            break;
        case ASSIGN_K:
            cout << "assign" << endl;
            break;
        case SUBROUTINE_BODY_K:
            cout << "subroutine_body" << endl;
            break;
        }
        printSyntaxTree(tree->child[0]);
        printSyntaxTree(tree->child[1]);
        printSyntaxTree(tree->child[2]);
        printSyntaxTree(tree->child[3]);
        printSyntaxTree(tree->child[4]);
        tree = tree->next;
    }
    indentno -= 2;
}

Parser::TreeNode * Parser::getSyntaxTree()
{
    return syntaxTree;
}
