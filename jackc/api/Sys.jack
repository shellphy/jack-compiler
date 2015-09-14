class Sys 
{
    /** 调用其他OS类的init函数, 进而调用Main.main()函数. */
    function void init() 
    {
        Memory.init();
        Main.main();
    /*
        Math.init();
        Screen.init();
        Output.init();
        Keyboard.init();
        Main.main();
        Sys.halt();
     */   
        return;
    }

    /** 中止程序执行 */
    function void halt() 
    {
        Output.printString("Program ended!");
        return;
    }

    /** 等待大约duration毫秒后返回 */
    function void wait(int duration) 
    {
        int temp;
        if(duration < 0)
        {
            Sys.error(1);
        }
        while(duration > 0)
        {
            temp = 50;
            while(temp > 0)
            {
                temp = temp - 1;
            }
            duration = duration - 1;
        }
        return;
    }

    /** 在屏幕上打印错误代码, 并中止程序执行 */
    function void error(int errorCode) 
    {
        String s;
        s = String.new(3);
        s = "ERR";
        Output.printString(s);
        Output.printInt(errorCode);
        Sys.halt();
        return;
    }
}
