class Math 
{
    /** 仅供内部使用 */
    function void init() 
    {
		return;
    }

    /** 返回x的绝对值    */
    function int abs(int x) 
    {
		int absNum;
		if (x < 0)
        {
			absNum = -x;
		}
		else
        {
			absNum = x; 
		}
		return absNum;
    }

    /** 返回x和y的乘积 */
    function int multiply(int x, int y) 
    {
		int sum;
		int shiftedX,functionY;
		int flag,j;
		boolean WhetherNeg;
		
        sum = 0;
		shiftedX = Math.abs(x);
		functionY= Math.abs(y);
		flag = 1;
		j = 0;
		if ((x == 0) | (y == 0)) 
        {
			return 0;
		}
        WhetherNeg = ((x < 0) == (y < 0));
		while(j < 16)
        {
			if((functionY & flag) == flag)
            {
				sum = sum + shiftedX;
			}
			shiftedX = shiftedX + shiftedX;
			flag = flag + flag;
			j = j + 1;
		}
		if (~WhetherNeg)
        {
            sum=-sum;
		}
		return sum;
	}

    /** 返回x/y的整数部分(x>0, y>0) */
	function int div(int x, int y) {
		int q, qy;
		if((y < 0) | (y > x))
        {
			return 0;
		}
		q = Math.div(x ,y + y);
		qy = Math.multiply(q, y);
		if (x - qy - qy < y)
        {
			return q+q;
		}
		else
        {
			return q + q + 1;
		}
	}
	
    /** 返回x/y的整数值    */
	function int divide(int x, int y) 
    {
		int answer;
		int absX,absY;
		boolean WhetherNeg;
		absX = Math.abs(x);
		absY= Math.abs(y);
		if(absY == 0)
        {
			return Sys.error(3);
		}
		WhetherNeg = ((x < 0) == (y < 0));
		answer = Math.div(absX, absY);
		if (~WhetherNeg)
        {
			answer = -answer;
		}
		return answer;
	}

    /** 返回x <= 2^n中指数n的值 */
	function int logTwo(int x)
    {
		int powerTwo,flag;
		if ((x > 16384) & ((x < 32767) | (x == 32767)))
        {
			return 15;
		}
		powerTwo = 1;
		flag = 0;
		while (powerTwo < x)
        {
			powerTwo = powerTwo + powerTwo;
			flag = flag + 1;
		}
		return flag;
	}
	
    /** 返回x的y次方 */
	function int power(int x, int y)
    {
		int flag;
		int result;
		
        flag = y;
		result = 1;
		
        if(y == 0)
        {
			return 1;
		}
		while (flag > 0)
        {
			result = Math.multiply(result, x);
			flag = flag - 1;
		}
		return result; 
	}	
	
    /** 返回x的平方根的整数部分 */
    function int sqrt(int x) 
    {
		int y, j, flag, powerJ;
        int n, halfN;
		
        y = 0;
		n = Math.logTwo(x);
		halfN = Math.divide(n,2);
		j = halfN;
		if (x < 0)
        {
			return Sys.error(3);
		}
		while (j > -1)
        {
			powerJ = Math.power(2, j);
			flag = y + powerJ;
			flag = Math.multiply(flag, flag);
			if (((flag < x) | (flag == x)) & (flag > 0))
            {
				y = y + powerJ;
			}
			j = j - 1;
		}
		return y;
    }

    /** 返回x和y中的较大值 */
    function int max(int a, int b) 
    {
		if (a > b)
        {
			return a;
		}
		else
        {
			return b;
		}
    }

    /** 返回x和y中的较小值 */
    function int min(int a, int b) 
    {
		if (a < b)
        {
			return a;
		}
		else
        {
			return b;
		}
    }
}
