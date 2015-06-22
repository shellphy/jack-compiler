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
          * [语法规则](#语法规则)
          * [语法树](#语法树)
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

    bool  true  false  if  else  while  int  float  void  return  string  char  break  continue
    
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
语法分析器有两个任务:  
    1, 判断源程序是否符合语法规则
    2, 生成抽象语法树

<a name = "语法规则">
####C-Mini语言的语法
C-Mini语言的语法由如下的上下文无关文法(BNF)定义.  
非粗体字表示非终结符, 粗体字表示终结符
<pre>
    program -> declaration_list
    declaration_list -> declaration_list declaration
                      | declaration
    declaration -> var_declaration
                 | function_declaration
                 | const_declaration
    var_declaration -> type <strong>ID</strong> <strong>;</strong>
                     | type <strong>ID</strong> <strong>[</strong> <strong>INT</strong> <strong>]</strong> <strong>;</strong>
    type -> <strong>int</strong>
          | <strong>float</strong>
          | <strong>void</strong>
          | <strong>string</strong>
          | <strong>char</strong>
          | <strong>bool</strong>
    function_declaration -> type <strong>ID</strong> <strong>(</strong> params <strong>)</strong> compound_statement 
    params -> param_list 
            | <strong>void</strong>
            | empty
    param_list -> param_list , param
                | param	
    param -> type <strong>ID</strong>
           | type <strong>ID [ ]</strong>
    compound_statement -> <strong>{</strong> local_declarations statement_list <strong>}</strong>	
    local_declarations -> local_declarations var_declaration
                        | empty
    statement_list -> statement_list statement
                    | empty
    statement -> expression_statement
               | compound_statement
               | selection_statement
               | iteration_statement
               | return_statement
               | assign_statement
    expression_statement -> expression <strong>;</strong> 
                          | empty <strong>;</strong>
    selection_statement -> <strong>if (</strong> expression <strong>)</strong> statement
                         | <strong>if (</strong> expression <strong>)</strong> statement <strong>else</strong> statement
    iteration_statement -> <strong>while (</strong> expression <strong>)</strong> statement
    return_statement -> <strong>return ;</strong> 
                      | <strong>return</strong> expression <strong>;</strong>
    expression -> additive_expression relational_operator additive_expression
                | additive_expression
    assign_statement -> var = expression ;
    var -> <strong>ID</strong>
         | <strong>ID [</strong> expression <strong>]</strong>
    relational_operator -> <strong><=</strong> 
                         | <strong>>=</strong>
                         | <strong>==</strong>
                         | <strong><</strong>
                         | <strong>></strong>
                         | <strong>!=</strong>
                         | <strong>&&</strong>
                         | <strong>||</strong>
    additive_expression -> additive_expression <strong>+</strong> term
                         | additive_expression <strong>–</strong> term
                         | term
    term -> term <strong>*</strong> factor
          | term <strong>/</strong> factor
          | factor
    factor -> ( expression )
            | var
            | call
            | <strong>INT</strong>
            | <strong>FLOAT</strong>
            | <strong>CHAR</strong>
            | <strong>STRING</strong>
    call -> <strong>ID (</strong> args <strong>)</strong>
    args -> arg_list 
          | empty
    arg_list -> arg_list <strong>,</strong> expression
              | expression
</pre>

    1, 每个程序都是由一个声明列表组成
    2, 声明列表包括变量声明和函数声明,这里的函数声明本质上应该是函数定义,也就是说函数必须是下面的形式:  
```C++
int func(int a, int b)
{
   ......
}
```

    而不能是下面的形式:
```C++
int func(int a, int b);
```

    3, 变量声明可以是单值变量的声明也可以是数组的声明,但是不能在声明的时候赋值,必须先声明, 并且一定要在一个语句块内部赋值  
    4, 每个函数必须要有返回类型, 参数空着不写,也可以显示加上void
    5, 


<a name = "语法树">
####语法树

<a name = "语义分析器"/>
### 语义分析器
类型检查

<a name = "其他"/>
## 其他
后续工作

