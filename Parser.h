#ifndef _PARSER_H
#define _PARSER_H

#include "Scanner.h"
#include <string>
#include <deque>
#include <set>

using namespace std;

class Parser
{
public:
    enum NodeKind {
        CLASS_K, CLASS_VAR_DEC_K, SUBROUTINE_DEC_K, BASIC_TYPE_K, CLASS_TYPE_K, NULL_K,
        PARAM_K, VAR_DEC_K, ARRAY_K, VAR_K, IF_STATEMENT_K, WHILE_STATEMENT_K, CALL_EXPRESSION_K,
        RETURN_STATEMENT_K, CALL_STATEMENT_K, BOOL_EXPRESSION_K,FUNCTION_CALL_K,CONSTRUCTOR_CALL_K,
        COMPARE_K, OPERATION_K, BOOL_K, ASSIGN_K, SUBROUTINE_BODY_K, BOOL_CONST_K, NEGATIVE_K,METHOD_CALL_K,
        INT_CONST_K, CHAR_CONST_K, STRING_CONST_K, KEY_WORD_CONST, THIS_K
    };
    class TreeNode
    {
    public:
        Scanner::Token token;
        TreeNode *child[5];
        TreeNode *next;
        NodeKind nodeKind;
        TreeNode()
        {
            child[0] = child[1] = child[2] = child[3] = child[4] = nullptr;
            next = nullptr;
        }
    };
private:
    set<string> filenames;
    string currentParserFilename;
    bool error;
    TreeNode *syntaxTree;
    Scanner scanner;
    bool haveReturnStatement;

    Scanner::Token getToken();
    Scanner::Token ungetToken();    
    deque<Scanner::Token> tokenBuffer1;
    deque<Scanner::Token> tokenBuffer2;
    string getFullName(string name);

    unsigned int errorNumbers;
    void syntaxError(string expected, Scanner::Token);

    TreeNode * parse_class_list();
    TreeNode * parse_class();
    TreeNode * parse_class_var_dec_list();
    TreeNode * parse_class_var_dec();
    TreeNode * parse_var_name_list();
    TreeNode * parse_type();
    TreeNode * parse_subroutine_dec_list();
    TreeNode * parse_subroutin_dec();
    TreeNode * parse_params();
    TreeNode * parse_param_list();
    TreeNode * parse_param();
    TreeNode * parse_subroutine_body();
    TreeNode * parse_var_dec_list();
    TreeNode * parse_var_dec();
    TreeNode * parse_statements();
    TreeNode * parse_statement();
    TreeNode * parse_assign_statement();
    TreeNode * parse_left_value();
    TreeNode * parse_if_statement();
    TreeNode * parse_while_statement();
    TreeNode * parse_return_statement();
    TreeNode * parse_call_statement();
    TreeNode * parse_expressions();
    TreeNode * parse_expression_list();
    TreeNode * parse_expression();
    TreeNode * parse_bool_expression();
    TreeNode * parse_additive_expression();
    TreeNode * parse_term();
    TreeNode * parse_factor();
    TreeNode * parse_positive_factor();
    TreeNode * parse_not_factor();
    TreeNode * parse_call_expression();

    void printSyntaxTree(TreeNode *tree);
public:
    Parser(set<string> filenames);
    bool hasError();
    TreeNode *getSyntaxTree();
    void print();
    void parse_program();
    static string getCallerName(string fullName);
    static string getFunctionName(string fullName);
};

#endif
