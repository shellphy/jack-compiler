# jack-Compiler 

### 效果
#####Demo1:
```C++
class Main 
{
    function void main() 
    {
        String s;
        
        Output.printString("Hello, world!");
        Output.println();
        
        Output.printString("What's your name?");
        Output.println();
        s = Input.readLine();
        Output.printString("Your name is: ");
        Output.printString(s);
        Output.println();
        
        return;
    }

}
```
#####运行结果:
![图片4](https://github.com/Xiang1993/C-Mini-Compiler/blob/master/folder/4.jpg)
   

#####Demo2
```C++
class Main 
{
    function void main() 
    {
		Array arr;
		String s;
		int i;
		
		arr = Array.new(5);		// 创建一个大小为5的数组
		i = 0;
		while (i < 5)
		{
			s = Input.readLine();
			arr[i] = s.intValue();
			i = i + 1;
		}
		
		Main.bubble_sort(arr, 5);
		
		i = 0;
		while (i < 5)
		{
			Output.printInt(arr[i]);
			i = i + 1;
		}
		Output.println();
		
		return;
	}
	
	/* 冒泡排序 */
	function void bubble_sort(Array arr, int n)
	{
		int i, j, tmp;
		i = n - 1;
		
		while (i > 0 | i == 0)		// 由于还没有加上 >= 运算符, 所以暂时用这个代替
		{
			j = 0;
			while (j < i)
			{
				if (arr[j] > arr[j + 1])
				{
					tmp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = tmp;
				}
				j = j + 1;
			}
			i = i - 1;
		}
	
		return;
	}
}
```
#####运行结果:
![图片6](https://github.com/Xiang1993/C-Mini-Compiler/blob/master/folder/6.png)

#####Demo3:
```C++
class Main
{
	function void main()
	{
		int a, b, c;
		String s;
		
		s = Input.readLine();
		a = s.intValue();
		
		s = Input.readLine();
		b = s.intValue();
		
		c = Main.gcd(a, b);   
		
		Output.printInt(c);
		Output.println();
		
		return;
	}
	
	// 求最大公约数
	function int gcd(int a, int b)
	{
		if (b == 0)
		{
			return a;
		}
		else
		{
			return Main.gcd(b, a - a / b * b);
			/* a - a / b * b相当于 a % b */
		}
	}
	
}
```
#####运行结果:
![图片5](https://github.com/Xiang1993/C-Mini-Compiler/blob/master/folder/5.png)



##<a name = "index"/>目录
* [背景介绍](#背景介绍)
* [jack语言介绍](#jack语言介绍)
    * [语法要素](#语法要素)
    * [程序结构](#程序结构)
    * [变量](#变量)
    * [语句](#语句)
    * [表达式](#表达式)
    * [jack标准库](#jack标准库)
         * [Math类](#Math类)
         * [String类](#String类)
         * [Array类](#Array类)
         * [Output类](#Output类)
         * [Input类](#Input类)
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
    * [符号表](#符号表)
    * [语义分析器](#语义分析器)
          * [语义规则](#语义规则)
    * [虚拟机代码生成](#虚拟机代码生成)
* [虚拟机](#虚拟机)

<a name = "背景介绍"/>
#背景介绍
去年学了编译原理,但是这门课的理论太多了,而且很难,学得是云里雾里.网上很多大神说学了编译原理之后最好能够实际动手做一个编译器出来,这样对能力有很大的提升.于是就下了定决心,带着写一个编译器的目的来重新学习编译原理.然后开始找公开课,买书,就这样开始了.

<a name = "jack语言介绍">
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
1, jack的基本编程单元是类, 每个类存在于独立的文件中, 可以单独编译, 下面是类的定义形式: 
    
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
3, jack必须至少包含一个Main类, 而且在Main类中必须包含一个function void main() 函数

<a name = "变量"/>
### 变量
1, 变量分类  
    
    jack中有四种变量类型: 成员变量, 静态变量, 局部变量和参数变量  
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
    
    jack是弱类型语言, 没有禁止不同类型之间的转换

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

* 常数
* 在作用域内的变量名(变量可以是静态、局部、成员或参数类型)
* 关键字this, 引用当前对象 (不能用于函数中)
* 数组语法是: 数组名称[表达式], 其中数组名称是Array类型的变量名
* 返回值为非空类型的子程序调用
* 一元运算符 "-" 或 "~" 作前缀的表达式
** - 表达式: 算术求反
** ~ 表达式: 布尔求反
* 形如 "表达式 运算符 表达式" 的表达式, 其中运算符可以是以下二元运算符中的一种;
*** +  -  *  /  &  |  <=  < >=  >  ==
* (表达式): 位于圆括号内的表达式

<a name = "标准库"/>
### 标准库
  标准库包括下面的类
  
    Math        提供基本的数学运算
    String      实现字符串String类型和字符串相关操作
    Array       实现数组Array类型和数组相关操作
    Output      处理屏幕上的文字输出
    Input       处理键盘的输入
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

<a name = "Input类"/>
#### Input类
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
在linux下运行compiler.sh或者make就可以变一处jackc.exe和jack.exe了

<a name = "模块介绍"/>
## 模块介绍
jack编译器主要有词法分析器,语法分析器,语义分析器,vm代码生成 和 虚拟机

<a name = "词法分析器"/>
### 词法分析器
词法分析器的源代码为Scanner.cpp  使用的手工编码的方法实现的  
&nbsp;&nbsp;&nbsp;&nbsp;词法分析器的主要任务是识别源程序中的单词(Token),假如有下面的C代码:
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
####jack语言的语法
jack语言的语法由如下的上下文无关文法(BNF)定义.  
非粗体字表示非终结符, 粗体字表示终结符
<pre>
    program -> classlist
    classlist -> classlist class
               | class
    class -> <strong>class</strong> ID <strong>{</strong> classVarDecList subroutineDecList <strong>}</strong>
    classVarDecList -> classVarDecList classVarDec
             	     |
    classVarDec -> <strong>static</strong> type varNameList <strong>;</strong>
                 | <strong>field</strong> type varNameList <strong>;</strong>
    varNameList -> varNameList <strong>,</strong> ID
                 | ID
    type -> <strong>int</strong>
          | <strong>float</strong>
          | <strong>char</strong>
          | <strong>boolean</strong>
          | <strong>void</strong>
          | ID
    subroutineDecList -> subroutineDecList subroutineDec
                       | 
    subroutineDec -> <strong>constructor</strong> type ID <strong>(</strong> params <strong>)</strong> subroutineBody
                   | <strong>function</strong> type ID <strong>(</strong> params <strong>)</strong> subroutineBody
                   | <strong>method</strong> type ID <strong>(</strong>params <strong>)</strong> subroutineBody
    params -> paramList
            | 
    paramList -> paramList <strong>,</strong> param
               | param
    param -> type ID
    subroutineBody -> <strong>{</strong> varDecList statements <strong>}</strong>
    varDecList -> varDecList varDec
                | 
    varDec -> type varNameList <strong>;</strong>
    statements -> statements statement
                | 
    statement -> assign_statement
               | if_statement
               | while_statement
               | return_statement
               | call_statement <strong>;</strong>
    assign_statement -> leftValue <strong>=</strong> expression <strong>;</strong> 
    leftValue -> ID
               | ID <strong>[</strong> expression <strong>]</strong>
    if_statement -> <strong>if (</strong> expression <strong>)</strong> statement
                  | <strong>if (</strong> expression <strong>)</strong> statement <strong>else</strong> statement
    while_statement -> <strong>while (</strong> expression <strong>) {</strong> statement <strong>}</strong>
    return_statement -> <strong>return ; </strong>
                      | <strong>return</strong> expression <strong>;</strong>
    call_statement -> ID <strong>(</strong> expressions <strong>)</strong> 
                    | ID <strong>.</strong> ID <strong>(</strong> expressions <strong>)</strong>
    expressions -> expression_list
                 | 
    expression_list -> expression_list <strong>,</strong> expression
                     | expression
    expression -> expression <strong>&</strong> boolExpression
                | expression <strong>|</strong> boolExpression
                | boolExpression
    boolExpression -> additive_expression relational_operator additive_expression
                    | additive_expression
    relational_operator -> <strong><=</strong> 
                         | <strong>>=</strong>
                         | <strong>==</strong>
                         | <strong><</strong>
                         | <strong>></strong>
                         | <strong>!=</strong>
    additive_expression -> additive_expression <strong>+</strong> term
                         | additive_expression <strong>–</strong> term
                         | term    
    term -> term <strong>*</strong> factor
          | term <strong>/</strong> factor
          | factor
    factor -> <strong>-</strong> positive_factor
            | positive_factor
    positive_factor -> <strong>~</strong> not_factor
                     | not_factor
    not_factor -> <strong>INT_CONST</strong>
                | <strong>CHAR_CONST</strong>
                | <strong>STRING_CONST</strong>
                | keywordConstant
                | ID
                | ID <strong>[</strong> expression <strong>]</strong>
                | call_expression
                | <strong>(</strong> expression <strong>)</strong>
    keywordConstant -> <strong>true</strong>
                     | <strong>false</strong>
                     | <strong>null</strong>
                     | <strong>this</strong>
    call_expression -> ID <strong>(</strong> expression <strong>)</strong>
                     | ID <strong>.</strong> ID <strong>(</strong> expression <strong>)</strong>
</pre>

<a name = "语法树">
####语法树
树的节点类型:  

<a name = "语义分析器"/>
### 语义分析器

<a name = "语义规则"/>
####语义规则

<a name = "符号表"/>
####符号表

<a name = "虚拟机"/>
####虚拟机

