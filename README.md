# C-Mini-Compiler 

##<a name = "index"/>目录
* [背景介绍](#背景介绍)
* [java--语言介绍](#java--语言介绍)
    * [语法要素](#语法要素)
    * [程序结构](#程序结构)
    * [变量](#变量)
    * [语句](#语句)
    * [表达式](#表达式)
    * [java--标准库](#java--标准库)
         * [Math类](#Math类)
         * [String类](#String类)
         * [Array类](#Array类)
         * [Output类](#Output类)
         * [Screen类](#Screen类)
         * [Keyboard类](#Keyboard类)
         * [Memory类](#Memory类)
         * [Sys类](#Sys类)
    * [Demo](#Demo)
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
          * [语义规则](#语义规则)
          * [符号表](#符号表)
* [其他](#其他)

<a name = "背景介绍"/>
#背景介绍
去年学了编译原理,但是这门课的理论太多了,而且很难,学得是云里雾里.网上很多大神说学了编译原理之后最好能够实际动手做一个编译器出来,这样对能力有很大的提升.于是就下了定决心,带着写一个编译器的目的来重新学习编译原理.然后开始找公开课,买书,就这样开始了.

刚开始买的是龙书,这本书太难了,看得那过程太痛苦了.又到网上找了本<编译原理与实践>,这本书里有一个实际的tiny语言编译器.看了之后我发现实际的编译器开发工作并没有想象中的那么复杂,有些复杂的理论并不是必须的.
又买了本<计算机系统要素>, 这本书里面有一个编译器的项目指导说明.
于是便按照这里面的项目要求来写编译器了. 书里面要编译的语言叫做"jack", 我给这门语言稍微做了些修改和扩展, 
由于这个语言和java很像, 差不多是java的一个子集, 所以给这门语言取名叫做java--

<a name = "java--语言介绍">
##java--语言介绍
### 语法要素
1, 保留字:   

    class, constructor, method, function, int, boolean, char, void, 
    static, field, if, else, while, return, true, false, null, this  
2, 标识符: 

    由字母或下划线开头, 后接任意任意个字母或数字或下划线  
3, 常量:   

    int类型的常数规定都是正整数, 没有负整数, 但是可以在正整数前面加上负号, 这是对正整数取负值的一元运算表达式  
    String类型的常量是把一个字符串用一对双引号括起来, 与java和C里面的字符串一样  
    boolean类型的常量可以是true和false  
    常数null表示一个空引用, 实际上就是数值0  
4, 合法的符号:  

    (   )   [   ]   ,   .   ;   =    +   -   *   /   &   |   ~   <   >   <=   >=   ==   
5, 注释:  

    与C语言和java一样, 支持两种注释形式, 单行注释//   和多行注释 /*  */  

<a name = "程序结构"/>
### 程序结构
1, java--的基本编程单元是类, 每个类存在于独立的文件中, 可以单独编译, 下面是类的定义形式: 
    
    class 类名
    {
    	成员变量(field)声明 和 静态变量(static)声明	// 比如放在子程序声明之前
    	子程序声明	// 子程序声明可以是构造函数声明(construtor), 静态函数声明(function)和方法声明(method)
    }
    
2, 子程序声明:
    
    subroutine 类型 名称 (参数列表)
    {
    	局部变量声明
    	语句
    }
3, java--必须至少包含一个Main类, 而且在Main类中必须包含一个function void main() 函数

<a name = "变量"/>
### 变量
1, 变量分类  
    
    java--中有四种变量类型: 成员变量, 静态变量, 局部变量和参数变量  
    成员变量通过field关键字来声明  
    静态变量通过static来声明
    在函数体的开始声明的变量是局部变量  
    在函数声明中声明的变量是参数变量  

2, 数据类型  
    
    基本数据类型和对象类型

3, 基本类型
    
    int, boolean, char
4, 对象类型
    
    同java一样, 声明一个对象实际上只是创建一个指向该对象的引用
5, 数组
    
    数组是通过内置类Array类声明的, 用Array声明的对象也是一个引用, 指向堆内存. 
    对数组的引用可以与传统的一样
        Array arr;
        arr[3] = 4;
    不支持多维数组. 
6, 字符串
    
    字符串是通过内置类String类来声明的, 同样, 用String声明的对象也是一个引用, 指向堆内存, 例如:
        String s;
        char c;
        s = String.new("hello, world!\n");
        c = s.charAt(4);
7, 类型转换
    
    java--是弱类型语言, 没有禁止不同类型之间的转换

<a name = "语句"/>
### 语句
1, 赋值语句  

    变量 = 表达式
    变量[表达式] = 表达式    
2, if语句

    if(表达式)                     // 不能省略大括号
    {
    	语句             
    }
    else
    {
        语句
    }
3, while语句

    while(表达式)
    {
        语句
    }
4, 函数调用语句

    方法名(表达式)
    类名.函数名(表达式)
5, return语句

    return 表达式
    return ;                     // 即使子程序返回void, 也要有return语句

<a name = "表达式"/>
### 表达式
java--表达式必须是下列之一:

    常数
    在作用域内的变量名(变量可以是静态、局部、成员或参数类型)
    关键字this, 引用当前对象 (不能用于函数中)
    数组语法是: 数组名称[表达式], 其中数组名称是Array类型的变量名
    返回值为非空类型的子程序调用
    一元运算符 "-" 或 "~" 作前缀的表达式
        - 表达式: 算术求反
        ~ 表达式: 布尔求反
    形如 "表达式 运算符 表达式" 的表达式, 其中运算符可以是以下二元运算符中的一种;
        +  -  *  /  &  |  <=  < >=  >  ==
    (表达式): 位于圆括号内的表达式

<a name = "标准库"/>
### 标准库
  标准库包括下面的类
  
    Math        提供基本的数学运算
    String      实现字符串String类型和字符串相关操作
    Array       实现数组Array类型和数组相关操作
    Output      处理屏幕上的文字输出
    Screen      处理屏幕上的图像输出
    Keyboard    处理键盘的用户输入
    Memory      处理内存操作
    Sys         提供与程序执行相关的服务

<a name = "Math类"/>
#### Math类
该类实现各种数学运算操作

<a name = "String类"/>
#### String类
该类实现String数据类型以及与字符串相关的操作

<a name = "Array类"/>
#### Array类
该类构造和清楚数组

<a name = "Output类"/>
#### Output类
该类提供在屏幕上打印文本的服务

<a name = "Screen类"/>
#### Screen类
该类提供在屏幕上绘制图形的服务

<a name = "Keyboard类"/>
#### Keyboard类
该类提供从标准键盘上读取输入的服务

<a name = "Memory类"/>
#### Memory类
该类允许直接访问宿主平台的主内存的服务

<a name = "Sys类"/>
#### Sys类
该类提供与程序指向相关的服务

<a name = "Demo"/>
### Demo

<a name = "项目介绍"/>
##项目介绍

<a name = "使用说明/">
## 使用说明
本项目是在Windows平台下用VS2013开发的,编译之后生成javac--.exe可执行文件
javac-- Main.j  
即可以对Main.j源文件编译,如果没有报错,就可以生成一个可执行文件Main.exe

<a name = "模块介绍"/>
## 模块介绍
与其他编译器类似,java--编译器主要有词法分析器模块,语法分析器模块,语义分析器模块,中间代码生成模块, 虚拟机模块, 目标代码生成模块

<a name = "词法分析器"/>
### 词法分析器
词法分析器的源代码为Scanner.cpp  使用的手工编码的方法实现的  
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

    +  -  *  /   <   <=  >  >=  ==  =  !=  ;  ,  (  )  [  ]  {  }  /*  */  “   &&   ||
    
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
语法分析器的源代码文件是Parser.cpp    使用的是递归下降的方法实现的  
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
    expression -> expression <strong>&&</strong> boolexpression
                | expression <strong>||</strong> boolexpression
                | boolexpression
    boolexpression -> additive_expression relational_operator additive_expression
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
                         | <strong>=</strong>
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
            | <strong>BOOL</strong>
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

    3, 变量声明可以是单值变量的声明也可以是数组的声明,但是不能在声明的时候赋值,必须先声明, 
       并且一定要在一个语句块内部赋值 
    4, 每个函数必须要有返回类型, 参数空着不写,也可以显示加上void
    5, 所有的运算都是左结合的
    6, 优先级(由低到高)
        && || 
        =  ==  <=  >=  ==  !=  <  >
        +  -
        *  /
        ()


<a name = "语法树">
####语法树
树的节点类型:  
![](https://github.com/Xiang1993/C-Mini-Compiler/blob/master/folder/4.png)  
一个节点最多有5个孩子,用0到4表示,节点的类型nodeKind在Parser.h中声明为枚举类型NodeKind  
源程序的所有Token通过递归下降算法扫描一遍, 就会递归地构建一个语法树  
以下面的程序代码为例:  
```C++
int fact(int x)
{
	if (x > 1)
		return x * fact(x-1);
	else
		return 1;
}

int main(void)
{
	int x;
	x = read();
	if (x > 0)
		write(fact(x));
}
```
通过Parser之后生成的语法树为:  
![](https://github.com/Xiang1993/C-Mini-Compiler/blob/master/folder/5.png)  
(注: 如果看不清图片里面的内容,建议把图片另存为本地图片,然后再放大浏览)  

<a name = "语义分析器"/>
### 语义分析器

<a name = "语义规则"/>
####语义规则
* 每个变量必须先定义,后使用
* 作用域规则和C语言类似. 全局作用域,块作用域,变量隐藏等
* 同一作用域内的变量只能够声明一次
* 不能有相同的函数名
* 赋值语句的左侧变量的类型必须和右侧的表达式类型一致
* 不允许任何类型转换
* 对于字符串类型,所有运算都不支持
* 调用函数时,参数类型必须相同,参数的个数必须相等
* 布尔类型变量只能够和布尔类型变量进行运算,且只能进行&&和||运算
* break语句和continue语句只能够出现在where语句里面
* ......待更新  


<a name = "符号表"/>
####符号表

建立了两个哈希表,一个是变量表, 一个是函数表
#####变量表
变量表的查找关键字为: string  
变量表的值是一个结构体, 第一个字段是类型, 第二个字段是一个布尔值, 如果这个值为true, 表示数组类型,否则就是普通的变量类型  
#####函数表
函数表的关键字为: string  
函数表的值是一个结构体, 第一个字段是类型, 跟变量表中的一样, 第二个字段是一个vector, 表示形参列表.  

<a name = "其他"/>
## 其他
后续工作

