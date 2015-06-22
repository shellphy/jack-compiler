# C-Mini-Compiler 

##<a name = "index"/>目录
* [背景介绍](#背景介绍)
* [项目介绍](#项目介绍)
* [使用说明](#使用说明)
* [模块介绍](#模块介绍)
    * [词法分析器](#词法分析器)
          * [词法规则](#词法规则)
          * [转移图](#转移图)
    * [语法分析器](#语法分析器)
    * [语义分析器](#语义分析器)
* [其他](#其他)

<a name = "背景介绍"/>
#背景介绍
去年学了编译原理,但是这门课的理论太多了,而且很难,学得是云里雾里.网上很多大神说学了编译原理之后最好能够实际动手做一个编译器出来,这样对能力有很大的提升.于是就下了定决心,带着写一个编译器的目的来重新学习编译原理.于是开始找公开课,买书,就这样开始了.

刚开始买的是龙书,这本书太难了,看得那过程太痛苦了.又到网上找了本<编译原理与实践>,这本书里有一个实际的tiny语言编译器.看了之后我发现实际的编译器开发工作并没有想象中的那么复杂,有些复杂的理论并不是必须的.里面还有一个C-Mini的编译器项目,于是便按照这里面的项目要求来写编译器了.

当然,我现在实现的这个编译器比书本里面的C-Mini项目做了一些的扩展,比如增加了float类型,string类型等

<a name = "项目介绍"/>
##项目介绍
C-Mini是C语言的一个子集,写这个编译器仅仅是为了加深对编译器内部工作原理的理解

<a name = "使用说明/">
## 使用说明
本项目是在Windows平台下用VS2013开发的,编译之后生成C-Mini.exe可执行文件.不可直接执行C-Mini.exe程序,需要在命令行下输入命令:  
C-Mini test.c  
即可以对test.c源文件编译,如果没有报错,就可以生成一个可执行文件test.exe

<a name = "模块介绍"/>
## 模块介绍
与其他编译器类似,C-Mini编译器主要有词法分析器模块,语法分析器模块,语义分析器模块,代码优化模块

<a name = "词法分析器"/>
### 词法分析器
&nbsp;&nbsp;&nbsp;&nbsp;词法分析器的主要任务是识别源程序中的单词(Token),假如有下面的代码:
```C++
int main()
{
    printf("Hello, world!\n");
    return 0;
}
```
&nbsp;&nbsp;&nbsp;&nbsp;通过词法分析器的扫描之后,返回的是一个一个单词(Token):

    关键字 int  
    标识符 main  
    左圆括号 '('  
    左花括号 '{'  
    标识符 printf
    左圆括号 '('  
    字符串 "Hello, world!\n"  
    右圆括号 ')'  
    分号 ';'  
    标识符 return  
    数字 0  
    右花括号 '}'



<a name = "词法规则"/>
#### 词法规则
首先定义一些词法规则,即这门语言对能够识别出来的单词,词法规则是用正则表达式来定义的

1,&nbsp;支持的字符集:

      ASCII码

2,&nbsp;标识符规则:

    digit = [0-9]  
    letter = [a-z]|[A-Z]  
    ID = letter(letter|digit)*
    
3,&nbsp;保留字:

    if  else  while  int  float  void  return  string  const  char  break  continue
    
4,&nbsp;整型数:

    INT = (+|-)?digit+
    
5,&nbsp;浮点数:

    FLOAT = interger(.digit+) | interger((E|e)digit+) | interger(.digit+)((E|e)digit+)
    
6,&nbsp;字符:

    以单引号开始,紧接着一个ASCII码字符,或者一个转义字符,最后以单引号结束.  
    转义字符有 \a, \b, \f, \n, \r, \t, \v, \\, \?, \’, \”

7,&nbsp;字符串:

    string = "((^")*(\”)*(^”)*)*"
    
8,&nbsp;合法的符号:

    +  -  *  /   <   <=  >  >=  ==  =  !=  ;  ,  (  )  [  ]  {  }  /*  */  “
    
<a name = "转移图"/>
####转移图
根据上面的词法规则可以画出状态转移图(FA),以方便编程

1,&nbsp;简单的转移图示例:

2,&nbsp;标识符,整型和浮点型的转移图:
![图片1](https://github.com/Xiang1993/C-Mini-Compiler/blob/master/folder/1.png)
3,&nbsp;字符串的转移图:
![图片2](https://github.com/Xiang1993/C-Mini-Compiler/blob/master/folder/2.png)
4,&nbsp;字符的转移图
![图片3](https://github.com/Xiang1993/C-Mini-Compiler/blob/master/folder/3.png)


<a name = "语法分析器"/>
### 语法分析器
C-Mini语言的语法由如下的上下文无关文法定义.

    1, program -> declaration_list
    2, declaration_list -> declaration_list declaration 
                         | declaration
    3, declaration -> var_declaration
                    | function_declaration
                    | const_declaration
    4, var_declaration -> type ID ;
                        | type ID [ INT ] ; 					// 声明并定义变量
    5, const_declaration -> const type ID ;
                          | const type ID [ INT ] ;		    // 声明并定义常量
    6, type -> int
             | float
             | void
             | string
             | char
7, function_declaration  type ID ( params ) compound_statement //声明并定义函数
8, params  param_list 
         | void
         | empty
9, param_list  param_list , param
           | param										// 参数列表
10, param  type ID
         | type ID [ ]										// 参数
11, compound_statement  { local_declarations statement_list }		// 复合语句
11, local_declarations  local_declarations var_declaration
                  | empty
12, statement_list  statement_list statement
               | empty
13, statement  expression_statement 		// 表达式语句
            | compound_statement			// 复合语句
            | selection_statement			// 选择语句
	        | iteration_statement			// 迭代语句
			| return_statement				// 返回语句
            | assign_statement				// 赋值语句
14, expression_statement  expression ; 
                      | ;
15, selection_statement  if ( expression ) statement
					| if ( expression ) statement else statement
17, iteration_statement  while ( expression ) statement
18, return_statement  return ; 
                  | return expression ;
19, expression  additive_expression relational_operator additive_expression
             | additive_expression
20, assign_statement  var = expression ;
21, var  ID
       | ID [ expression ]
22, relational_operator  <= 
                    | >=
				    | ==
					| <
 					| >
					| !=
23, additive_expression  additive_expression + term
				    | additive_expression – term
					| term
24, term  term * factor
	    | term / factor
		| factor
25, factor  ( expression )
         | var
		 | call
		 | INT
		 | FLOAT
		 | CHAR
         | STRING
26, call  ID ( args )
27, args  arg_list 
	    | empty
28, arg_list  arg_list , expression
          | expression


<a name = "语义分析器"/>
### 语义分析器
类型检查

<a name = "其他"/>
## 其他
后续工作

