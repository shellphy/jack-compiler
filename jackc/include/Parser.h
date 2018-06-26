#ifndef _PARSER_H
#define _PARSER_H

#include "Error.h"
#include "Scanner.h"
#include <deque>
#include <string>
#include <vector>

using namespace std;

class Parser {
public:
  enum NodeKind {
    None,
    CLASS_K,
    CLASS_VAR_DEC_K,
    SUBROUTINE_DEC_K,
    BASIC_TYPE_K,
    CLASS_TYPE_K,
    NULL_K,
    PARAM_K,
    VAR_DEC_K,
    ARRAY_K,
    VAR_K,
    IF_STATEMENT_K,
    WHILE_STATEMENT_K,
    CALL_EXPRESSION_K,
    RETURN_STATEMENT_K,
    CALL_STATEMENT_K,
    BOOL_EXPRESSION_K,
    FUNCTION_CALL_K,
    CONSTRUCTOR_CALL_K,
    COMPARE_K,
    OPERATION_K,
    BOOL_K,
    ASSIGN_K,
    SUBROUTINE_BODY_K,
    BOOL_CONST_K,
    NEGATIVE_K,
    METHOD_CALL_K,
    INT_CONST_K,
    CHAR_CONST_K,
    STRING_CONST_K,
    KEY_WORD_CONST,
    THIS_K
  };
  class TreeNode {
  public:
    Scanner::Token token;
    TreeNode *child[5];
    TreeNode *next;
    NodeKind nodeKind;
    TreeNode() {
      nodeKind = None;
      child[0] = child[1] = child[2] = child[3] = child[4] = nullptr;
      next = nullptr;
    }
  };

private:
  vector<string> filenames;
  string currentParserFilename;
  TreeNode *syntaxTree;
  Scanner scanner;
  bool haveReturnStatement; // 要保证每个函数都有return语句, 即使返回值为void

  Scanner::Token getToken();   // 从缓冲区中取出一个token
  Scanner::Token ungetToken(); // 把上一次取出的token放入到缓冲区中
  deque<Scanner::Token> tokenBuffer1; // 左缓冲区
  deque<Scanner::Token> tokenBuffer2; // 右缓冲区
  string getFullName(string name);    // 返回

  TreeNode *parseClassList();
  TreeNode *parseClass();
  TreeNode *parseClassVarDecList();
  TreeNode *parseClassVarDec();
  TreeNode *parseVarNameList();
  TreeNode *parseType();
  TreeNode *parseSubroutineDecList();
  TreeNode *parseSubroutinDec();
  TreeNode *parseParams();
  TreeNode *parseParamList();
  TreeNode *parseParam();
  TreeNode *parseSubroutineBody();
  TreeNode *parseVarDecList();
  TreeNode *parseVarDec();
  TreeNode *parseStatements();
  TreeNode *parseStatement();
  TreeNode *parseAssignStatement();
  TreeNode *parseLeftValue();
  TreeNode *parseIfStatement();
  TreeNode *parseWhileStatement();
  TreeNode *parseReturnStatement();
  TreeNode *parseCallStatement();
  TreeNode *parseExpressions();
  TreeNode *parseExpressionList();
  TreeNode *parseExpression();
  TreeNode *parseBoolExpression();
  TreeNode *parseAdditiveExpression();
  TreeNode *parseTerm();
  TreeNode *parseFactor();
  TreeNode *parsePositiveFactor();
  TreeNode *parseNotFactor();
  TreeNode *parseCallExpression();

  void printSyntaxTree(TreeNode *tree);

public:
  Parser(vector<string> &filenames);
  bool hasError();
  TreeNode *getSyntaxTree();
  void print();
  void parse_program();
  static string getCallerName(string const& fullName);
  static string getFunctionName(string const &fullName);
};

#endif
