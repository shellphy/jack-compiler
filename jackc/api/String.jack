class String 
{
    field Array a;            // 字符串本身是一个数组, a是这个数组的引用
    field int stringLength;   // 当前字符串的长度
    field boolean negFlag;    // 判断是否为负数
    field int allocLength;    // 最大字符串长度

    /** 构造一个长度为maxLength的字符串  */
    constructor String new(int maxLength) 
    {
        if (maxLength < 1)
        {
            maxLength = 1;
        }
        allocLength = maxLength;
        negFlag = false;
        a = Array.new(maxLength);
        stringLength = 0;
        return this;
    }
    
    /** 销毁字符串并释放内存 */
    method void dispose() 
    {
        a.dispose();
        return;
    }

    method int length() 
    {
        return stringLength;
    }

    /** 返回字符串第j个位置上的字符 */
    method char charAt(int j) 
    {
        char c;
        c = a[j];
        return c;
    }

    /** 将字符串中第j个元素置为字符c */
    method void setCharAt(int j, char c) 
    {
        a[j] = c;
        return;
    }

    /** 在字符串末尾追加字符c并返回整个字符串 */
    method String appendChar(char c) 
    {
        int length;
        if(stringLength == allocLength)
        {
            Sys.error(17);
        }
        length = stringLength;
        a[length] = c;
        stringLength = stringLength+1;
        return this;
    }

    /** 删除字符串中的最后一个字符 */
    method void eraseLastChar() 
    {
        int length;
        length = stringLength;
        stringLength = stringLength-1;
        return;
    }

    /** 从最左边开始直到遇到非数字字符为止的字串的整数值 */
    method int intValue() 
    {
        int length, i, result;
        int temp;
        boolean flag;   // 判断是否为负数
        
        flag = false;
        i = 0;
        length = stringLength;
        result = 0;
        if (a[0] == 45)
        {
            flag = true;
            i = i + 1;
        }
        while (i < length)
        {
            if ((a[i] > 47) & (a[i] < 58))
            {            
                temp = a[i] - 48;
                result = Math.multiply(result, 10) + temp;
                i = i + 1;
            }
            else
            {
                if (flag)
                {
                    result = -result;
                }
                return result;
            }
        }
        if (flag)
        {
            result = -result;
        }
        return result;
    }

    /** 以字符串形式保存number所代表的整数 */
    method void setInt(int number) 
    {
        int lastDigit;
        int divNumber, tenNumber;
        int c;
        
        stringLength = 0;
        if (number < 0)
        {
            negFlag = true;
            number = Math.abs(number);
        }
        divNumber = Math.divide(number, 10);
        tenNumber = Math.multiply(divNumber, 10);
        lastDigit = number - tenNumber;
        c = lastDigit + 48;
        if (number < 10)
        {
            if (negFlag)
            {
                appendChar(45);
                negFlag = false;
            }
            appendChar(c);
        }
        else
        {
            setInt(divNumber);
            appendChar(c);
        }
        return;
    }

    /** 返回换行符 */
    function char newLine() {
        return 10;
    }

    /** 返回双引号字符 */
    function char doubleQuote() {
        return 34;
    }
}
