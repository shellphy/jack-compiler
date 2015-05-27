学了编译原理之后, 一直都想实现一个编译器
所以, 我就开始设计一个精简版的C语言, 我称这个语言为C-Mini

#词法分析器
    字符集: ASCII码
    legal symbols 
        +   -   *   /   <   <=  >  >=  ==  =  !=  ;  ,  (  )  [  ]  {  }  /*  */   //
    white space
        ' '   '\n'   '\t'
    digit = [0-9]
    letter = [a-z]|[A-Z] 
    interger = (+|-)?digit+
    float = interger(.digit+) | interger((E|e)digit+) | interger(.digit+)((E|e)digit+)
    string = "(^")*"
    identifier = letter(letter|digit)*
    reserved words = if | else | while | int | float | void | return | string
