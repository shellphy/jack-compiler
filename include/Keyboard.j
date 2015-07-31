class Keyboard 
{
    /** 初始化, 仅供内部使用 */
    function void init() {
        return;
    } 

    /**
     * Returns the ASCII code (as char) of the currently pressed key,
     * or 0 if no key is currently pressed.
     * Recognizes all ASCII characters, as well as the following extension
     * of action keys:
     * New line = 128 = String.newline()
     * Backspace = 129 = String.backspace()
     * Left Arrow = 130
     * Up Arrow = 131
     * Right Arrow = 132
     * Down Arrow = 133
     * Home = 134
     * End = 135
     * Page Up = 136
     * Page Down = 137
     * Insert = 138
     * Delete = 139
     * ESC = 140
     * F1 - F12 = 141 - 152
     */
     
    /**	返回当前键盘上被按下的键所对应的字符; 如果当前没有键被按下则返回0 */
    function char keyPressed() 
    {  
        return Memory.peek(24576);
    }

    /** 等待键盘上某个键被按下后又被释放, 将该键的字符返回并显示到屏幕上 */
    function char readChar() 
    {
        char c;
        while(~(Keyboard.keyPressed()))
        {}
        c = Keyboard.keyPressed();
        while(c == Keyboard.keyPressed())
        {}
        Output.printChar(c);
        return c;
    }

    /**	从键盘输入读取一行字符串(直到遇见换行符为止), 在屏幕上显示该行字符串, 最后返回该字符串. 该函数能处理退格 */
    function String readLine(String message) 
    {
        String s;
        char c;
        
        Output.printString(message);
        s = String.new(100);
        while(true)
        {
            c = Keyboard.readChar();
            if (c == 128)
            {
                Output.printChar(128);
                return s;
            }
            if (c == 129)
            {
                s.eraseLastChar();
                Output.backSpace();
            }
            else
            {
                s = s.appendChar(c);
            }
        }
        return s;
    }   

    /**	
     * 从键盘输入读取一行字符串(直到遇到换行符为止), 在屏幕上显示该行字符串, 
     * 然后返回其对应的整数值(直到遇见第一个非数字字符为止).
     * 该函数也能处理退格
     */
    function int readInt(String message) 
    {
        String s;
        s = Keyboard.readLine(message);
        return s.intValue();
    }
}
