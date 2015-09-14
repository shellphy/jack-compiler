class Input
{
    function char readChar()
    {
        char ch;
        ch = IO.getchar();
        return ch;
    }
    
    function String readLine()
    {
        char ch;
        String s;
        s = String.new(100);
        while (true)
        {
            ch = IO.getchar();
            if (ch == 10)
            {
                return s;
            }
            s = s.appendChar(ch);
        }
        return s;
    }
}