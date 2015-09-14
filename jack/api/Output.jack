class Output 
{
    function void printChar(char ch)
    {
        IO.putchar(ch);
        return;
    }
    
    function void printString(String s) 
    {
        int strLength;
        char temp;
        int i;
        i = 0;
        strLength = s.length();
        while(i < strLength)
        {
            temp = s.charAt(i);
            Output.printChar(temp);
            i = i + 1;
        }
        return;
    }
    
    function void printInt(int i)
    {
        String str;
        str = String.new(6);
        str.setInt(i);
        Output.printString(str);
        return;
    }

    function void println() 
    {
        IO.putchar(10);
        return;
    }
}
