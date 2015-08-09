class Output 
{
    // Character map for printing on the left of a screen word
    static Array charMaps;
    static int cursorRow,cursorCol; //record the actual pixel line in 256*512
    static int charRow,charCol; //record the char line in 23*64

    /** 初始化屏幕并将光标的位置置于屏幕的左上角 */
    function void init() 
    {
        cursorRow = 0;
        cursorCol = 0;
        charRow = 0;
        charCol = 0;
        Output.initMap();
        return;
    }

    // 初始化字符位图
    function void initMap() 
    {
        int i;
    
        charMaps = Array.new(127);
        
        // black square (used for non printable characters)
        Output.create(0,63,63,63,63,63,63,63,63,63,0,0);

        // Assigns the bitmap for each character in the charachter set.
        Output.create(32,0,0,0,0,0,0,0,0,0,0,0);          //
        Output.create(33,12,30,30,30,12,12,0,12,12,0,0);  // !
        Output.create(34,54,54,20,0,0,0,0,0,0,0,0);       // "
        Output.create(35,0,18,18,63,18,18,63,18,18,0,0);  // #
        Output.create(36,12,30,51,3,30,48,51,30,12,12,0); // $
        Output.create(37,0,0,35,51,24,12,6,51,49,0,0);    // %
        Output.create(38,12,30,30,12,54,27,27,27,54,0,0); // &
        Output.create(39,12,12,6,0,0,0,0,0,0,0,0);        // '
        Output.create(40,24,12,6,6,6,6,6,12,24,0,0);      // (
        Output.create(41,6,12,24,24,24,24,24,12,6,0,0);   // )
        Output.create(42,0,0,0,51,30,63,30,51,0,0,0);     // *
        Output.create(43,0,0,0,12,12,63,12,12,0,0,0);     // +
        Output.create(44,0,0,0,0,0,0,0,12,12,6,0);        // ,
        Output.create(45,0,0,0,0,0,63,0,0,0,0,0);         // -
        Output.create(46,0,0,0,0,0,0,0,12,12,0,0);        // .    
        Output.create(47,0,0,32,48,24,12,6,3,1,0,0);      // /
        
        Output.create(48,12,30,51,51,51,51,51,30,12,0,0); // 0
        Output.create(49,12,14,15,12,12,12,12,12,63,0,0); // 1
        Output.create(50,30,51,48,24,12,6,3,51,63,0,0);   // 2
        Output.create(51,30,51,48,48,28,48,48,51,30,0,0); // 3
        Output.create(52,16,24,28,26,25,63,24,24,60,0,0); // 4
        Output.create(53,63,3,3,31,48,48,48,51,30,0,0);   // 5
        Output.create(54,28,6,3,3,31,51,51,51,30,0,0);    // 6
        Output.create(55,63,49,48,48,24,12,12,12,12,0,0); // 7
        Output.create(56,30,51,51,51,30,51,51,51,30,0,0); // 8
        Output.create(57,30,51,51,51,62,48,48,24,14,0,0); // 9
        
        Output.create(58,0,0,12,12,0,0,12,12,0,0,0);      // :
        Output.create(59,0,0,12,12,0,0,12,12,6,0,0);      // ;
        Output.create(60,0,0,24,12,6,3,6,12,24,0,0);      // <
        Output.create(61,0,0,0,63,0,0,63,0,0,0,0);        // =
        Output.create(62,0,0,3,6,12,24,12,6,3,0,0);       // >
        Output.create(64,30,51,51,59,59,59,27,3,30,0,0);  // @
        Output.create(63,30,51,51,24,12,12,0,12,12,0,0);  // ?

        Output.create(65,12,30,51,51,63,51,51,51,51,0,0);   // A ** TO BE FILLED **
        Output.create(66,31,51,51,51,31,51,51,51,31,0,0); // B
        Output.create(67,28,54,35,3,3,3,35,54,28,0,0);    // C
        Output.create(68,15,27,51,51,51,51,51,27,15,0,0); // D
        Output.create(69,63,51,35,11,15,11,35,51,63,0,0); // E
        Output.create(70,63,51,35,11,15,11,3,3,3,0,0);    // F
        Output.create(71,28,54,35,3,59,51,51,54,44,0,0);  // G
        Output.create(72,51,51,51,51,63,51,51,51,51,0,0); // H
        Output.create(73,30,12,12,12,12,12,12,12,30,0,0); // I
        Output.create(74,60,24,24,24,24,24,27,27,14,0,0); // J
        Output.create(75,51,51,51,27,15,27,51,51,51,0,0); // K
        Output.create(76,3,3,3,3,3,3,35,51,63,0,0);       // L
        Output.create(77,33,51,63,63,51,51,51,51,51,0,0); // M
        Output.create(78,51,51,55,55,63,59,59,51,51,0,0); // N
        Output.create(79,30,51,51,51,51,51,51,51,30,0,0); // O
        Output.create(80,31,51,51,51,31,3,3,3,3,0,0);     // P
        Output.create(81,30,51,51,51,51,51,63,59,30,48,0);// Q
        Output.create(82,31,51,51,51,31,27,51,51,51,0,0); // R
        Output.create(83,30,51,51,6,28,48,51,51,30,0,0);  // S
        Output.create(84,63,63,45,12,12,12,12,12,30,0,0); // T
        Output.create(85,51,51,51,51,51,51,51,51,30,0,0); // U
        Output.create(86,51,51,51,51,51,30,30,12,12,0,0); // V
        Output.create(87,51,51,51,51,51,63,63,63,18,0,0); // W
        Output.create(88,51,51,30,30,12,30,30,51,51,0,0); // X
        Output.create(89,51,51,51,51,30,12,12,12,30,0,0); // Y
        Output.create(90,63,51,49,24,12,6,35,51,63,0,0);  // Z

        Output.create(91,30,6,6,6,6,6,6,6,30,0,0);          // [
        Output.create(92,0,0,1,3,6,12,24,48,32,0,0);        // \
        Output.create(93,30,24,24,24,24,24,24,24,30,0,0);   // ]
        Output.create(94,8,28,54,0,0,0,0,0,0,0,0);          // ^
        Output.create(95,0,0,0,0,0,0,0,0,0,63,0);           // _
        Output.create(96,6,12,24,0,0,0,0,0,0,0,0);          // `

        Output.create(97,0,0,0,14,24,30,27,27,54,0,0);      // a
        Output.create(98,3,3,3,15,27,51,51,51,30,0,0);      // b
        Output.create(99,0,0,0,30,51,3,3,51,30,0,0);        // c
        Output.create(100,48,48,48,60,54,51,51,51,30,0,0);  // d
        Output.create(101,0,0,0,30,51,63,3,51,30,0,0);      // e
        Output.create(102,28,54,38,6,15,6,6,6,15,0,0);      // f
        Output.create(103,0,0,30,51,51,51,62,48,51,30,0);   // g
        Output.create(104,3,3,3,27,55,51,51,51,51,0,0);     // h
        Output.create(105,12,12,0,14,12,12,12,12,30,0,0);   // i
        Output.create(106,48,48,0,56,48,48,48,48,51,30,0);  // j
        Output.create(107,3,3,3,51,27,15,15,27,51,0,0);     // k
        Output.create(108,14,12,12,12,12,12,12,12,30,0,0);  // l
        Output.create(109,0,0,0,29,63,43,43,43,43,0,0);     // m
        Output.create(110,0,0,0,29,51,51,51,51,51,0,0);     // n
        Output.create(111,0,0,0,30,51,51,51,51,30,0,0);     // o
        Output.create(112,0,0,0,30,51,51,51,31,3,3,0);      // p
        Output.create(113,0,0,0,30,51,51,51,62,48,48,0);    // q
        Output.create(114,0,0,0,29,55,51,3,3,7,0,0);        // r
        Output.create(115,0,0,0,30,51,6,24,51,30,0,0);      // s
        Output.create(116,4,6,6,15,6,6,6,54,28,0,0);        // t
        Output.create(117,0,0,0,27,27,27,27,27,54,0,0);     // u
        Output.create(118,0,0,0,51,51,51,51,30,12,0,0);     // v
        Output.create(119,0,0,0,51,51,51,63,63,18,0,0);     // w
        Output.create(120,0,0,0,51,30,12,12,30,51,0,0);     // x
        Output.create(121,0,0,0,51,51,51,62,48,24,15,0);    // y
        Output.create(122,0,0,0,63,27,12,6,51,63,0,0);      // z
        
        Output.create(123,56,12,12,12,7,12,12,12,56,0,0);   // {
        Output.create(124,12,12,12,12,12,12,12,12,12,0,0);  // |
        Output.create(125,7,12,12,12,56,12,12,12,7,0,0);    // }
        Output.create(126,38,45,25,0,0,0,0,0,0,0,0);        // ~

        return;
    }

    /** 根据给定的值创建字符位图 */
    function void create(int index, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k) 
    {
        Array map;

        map = Array.new(11);
        charMaps[index] = map;

        map[0] = a;
        map[1] = b;
        map[2] = c;
        map[3] = d;
        map[4] = e;
        map[5] = f;
        map[6] = g;
        map[7] = h;
        map[8] = i;
        map[9] = j;
        map[10] = k;

        return;
    }
    
    // Returns the character map (array of size 11) for the given character
    // If an invalid character is given, returns the character map of a black square.
    function Array getMap(char c) 
    {
        if ((c < 32) | (c > 126)) 
        {
            c = 0;
        }
        return charMaps[c];
    }

    /** 将光标移动到第i行和第j列, 并且删除此位置上的字符 */
    function void moveCursor(int i, int j) 
    {
        boolean oddOrEven;
        int column, row, flag, tempVal;
        int divNum, address;
        int tempChar, oriChar, answer;
        Array zeroMap;
        flag = 0;
        if(i == 23)
        {
            cursorRow = 0;
            cursorCol = 0;
            charRow = 0;
            charCol = 0;           
        }
        else
        {
            cursorRow = Math.multiply(i, 11);
            cursorCol = Math.multiply(j, 8);   
            charRow = i;
            charCol = j;         
        }
        row = cursorRow;
        column = cursorCol;
        //define zeroMap[]
        zeroMap = Output.getMap(32);
        //find address
        divNum = Math.divide(column,16);
        address = 16384 + Math.multiply(row,32) + divNum;
        //print char 0
        tempVal = Math.multiply(Math.divide(charCol,2),2);
        if(tempVal == charCol)
        {
            oddOrEven=true;
        }
        else
        {
            oddOrEven = false;
        }
        if(~oddOrEven)
        {
            while(flag < 11)
            {
                tempChar = Math.multiply(zeroMap[flag], 256);                
                oriChar = Memory.peek(address);
                oriChar = (oriChar & 255);
                answer = tempChar + oriChar;
                Memory.poke(address, answer);
                address = address + 32;
                flag = flag + 1;
            }
        }
        else
        {
            while(flag < 11)
            {
                Memory.poke(address, zeroMap[flag]);
                address = address + 32;
                flag = flag + 1;
            }
        }
        return;
    }

    /** 在光标处打印字符c并在该行将光标向前移动一格 */
    function void printChar(char c) 
    {
        int row, column;
        int i, divNum, address, oriChar, tempChar, answer, tempVal;
        boolean oddOrEven;
        Array characterMap;
        i = 0;
        characterMap = Output.getMap(c);
        row = cursorRow;
        column = cursorCol;
        //find address
        divNum = Math.divide(column, 16);
        address = 16384 + Math.multiply(row, 32) + divNum;
        tempVal = Math.multiply(Math.divide(charCol,2), 2);
        if(tempVal == charCol)
        {
            oddOrEven = true;
        }
        else
        {
            oddOrEven = false;
        }
        if(~oddOrEven)
        {
            while(i < 11)
            {
                tempChar = Math.multiply(characterMap[i], 256);                
                oriChar = Memory.peek(address);
                answer = tempChar + oriChar;
                Memory.poke(address, answer);
                address = address + 32;
                i = i + 1;
            }
        }
        else
        {
            while(i < 11)
            {
                Memory.poke(address, characterMap[i]);
                address = address + 32;
                i = i + 1;
            }
        }
        if(charCol == 63)
        {
            Output.println();
            return;
        }
        else
        {
            charCol = charCol + 1;
            Output.moveCursor(charRow, charCol);
            return;            
        }
    }

    /** 在光标处开始打印字符串s, 并将光标向前移到打印结束的位置 */
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

    /** 在光标处开始打印整数i, 并且将光标向前移动到打印结束的位置 */
    function void printInt(int i) 
    {
        String str;
        str = String.new(6);
        str.setInt(i);
        Output.printString(str);
        return;
    }

    /** 将光标移动到下一行的起始处 */
    function void println() 
    {
        charRow = charRow+1;
        charCol = 0;
        Output.moveCursor(charRow, charCol);
        return;
    }

    /** 在该行将光标向后移动一格 */
    function void backSpace() 
    {
        boolean oddOrEven;
        int column,row,flag,tempVal;
        int divNum,address;
        int tempChar,oriChar,answer;
        Array zeroMap;
        if(charCol == 0)
        {
            if(charRow > 0)
            {
                charRow = charRow - 1;
                charCol = 63;
            }
        }
        else
        {
            charCol = charCol - 1;            
        }
        Output.moveCursor(charRow, charCol);
        return;
    }
}
